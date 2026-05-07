#!/usr/bin/env python3
# SPDX-License-Identifier: Apache-2.0

"""Create and update BitBox03 image headers."""

from __future__ import annotations

import argparse
import json
from pathlib import Path
from typing import Any, TypedDict, cast

HEADER_LEN = 1024
MARKETING_VERSION_LEN = 15
HASH_COUNT = 16
HASH_LEN = 32
RESERVED_LEN = 0x120
KEY_COUNT = 3
KEY_LEN = 64
INVALID_CODE_SIZE = 0xFFFF_FFFF


class HeaderManifest(TypedDict):
    magic: bytes
    hw_model: int
    signatures: bytes


def _read_json(path: Path) -> dict[str, Any]:
    with path.open("r", encoding="utf-8") as infile:
        return cast(dict[str, Any], json.load(infile))


def _encode_marketing_version(version: str) -> bytes:
    encoded = version.encode("ascii")
    if len(encoded) >= MARKETING_VERSION_LEN:
        raise ValueError("marketing version too long")
    return encoded + b"\x00" * (MARKETING_VERSION_LEN - len(encoded))


def _decode_hex_entries(entries: list[str], field_name: str) -> bytes:
    if len(entries) != KEY_COUNT:
        raise ValueError(f"{field_name} must contain exactly {KEY_COUNT} entries")
    output = bytearray()
    for index, entry in enumerate(entries):
        try:
            decoded = bytes.fromhex(entry)
        except ValueError as exc:
            raise ValueError(f"{field_name}[{index}] is not valid hex") from exc
        if len(decoded) != KEY_LEN:
            raise ValueError(f"{field_name}[{index}] must be {KEY_LEN} bytes")
        output.extend(decoded)
    return bytes(output)


def _load_header_manifest(path: Path) -> HeaderManifest:
    manifest = _read_json(path)
    magic = manifest.get("magic")
    if not isinstance(magic, str) or len(magic.encode("ascii")) != 4:
        raise ValueError("manifest field 'magic' must be a 4-byte ASCII string")
    hw_model = manifest.get("hw_model")
    if not isinstance(hw_model, int) or not 0 <= hw_model <= 0xFFFF_FFFF:
        raise ValueError("manifest field 'hw_model' must be a u32")
    return {
        "magic": magic.encode("ascii"),
        "hw_model": hw_model,
        "signatures": _decode_hex_entries(manifest.get("signatures", []), "signatures"),
    }


def _marketing_version(versions_manifest: Path, version_key: str) -> str:
    versions = _read_json(versions_manifest)
    version = versions.get(version_key)
    if not isinstance(version, str):
        raise ValueError(f"versions manifest does not contain string key '{version_key}'")
    return version


def build_header(
    *,
    manifest: HeaderManifest,
    marketing_version: str,
    monotonic_version: int,
    code_size: int,
) -> bytes:
    """Build an unsigned image header."""
    if not 0 <= monotonic_version <= 0xFF:
        raise ValueError("monotonic_version must be a u8")
    if not 0 <= code_size <= 0xFFFF_FFFF:
        raise ValueError("code_size must be a u32")

    header = bytearray()
    header.extend(manifest["magic"])
    header.extend(HEADER_LEN.to_bytes(4, "little"))
    header.extend(code_size.to_bytes(4, "little"))
    header.extend(manifest["hw_model"].to_bytes(4, "little"))
    header.extend(_encode_marketing_version(marketing_version))
    header.extend(monotonic_version.to_bytes(1, "little"))
    header.extend(b"\x00" * HASH_COUNT * HASH_LEN)
    header.extend(b"\xff" * RESERVED_LEN)
    header.extend(manifest["signatures"])
    if len(header) > HEADER_LEN:
        raise ValueError("header fields exceed 1024 bytes")
    header.extend(b"\x00" * (HEADER_LEN - len(header)))
    return bytes(header)


def finalize_header_code_size(header_bytes: bytes, code_size: int) -> bytes:
    if len(header_bytes) != HEADER_LEN:
        raise ValueError("header must be exactly 1024 bytes")
    if not 0 <= code_size <= 0xFFFF_FFFF:
        raise ValueError("code_size must be a u32")
    updated = bytearray(header_bytes)
    updated[8:12] = code_size.to_bytes(4, "little")
    return bytes(updated)


def finalize_code_size(header_bytes: bytes, payload: bytes) -> bytes:
    return finalize_header_code_size(header_bytes, len(payload))


def _read_u16(data: bytes, offset: int) -> int:
    return int.from_bytes(data[offset : offset + 2], "little")


def _read_u32(data: bytes, offset: int) -> int:
    return int.from_bytes(data[offset : offset + 4], "little")


def _validate_elf32_le(data: bytes, elf: Path) -> None:
    if data[:4] != b"\x7fELF":
        raise ValueError(f"{elf} is not an ELF file")
    if data[4] != 1 or data[5] != 1:
        raise ValueError(f"{elf} must be a little-endian ELF32 file")


def _elf_section(elf: Path, section_name: str) -> tuple[int, int, int]:
    data = elf.read_bytes()
    _validate_elf32_le(data, elf)

    section_header_offset = _read_u32(data, 32)
    section_header_size = _read_u16(data, 46)
    section_count = _read_u16(data, 48)
    section_names_index = _read_u16(data, 50)
    if section_header_size < 40:
        raise ValueError(f"{elf} has invalid section header size")
    if section_names_index >= section_count:
        raise ValueError(f"{elf} has invalid section name table index")

    def section_header(index: int) -> bytes:
        start = section_header_offset + index * section_header_size
        end = start + section_header_size
        if end > len(data):
            raise ValueError(f"{elf} section header table is truncated")
        return data[start:end]

    section_names_header = section_header(section_names_index)
    section_names_offset = _read_u32(section_names_header, 16)
    section_names_size = _read_u32(section_names_header, 20)
    section_names = data[section_names_offset : section_names_offset + section_names_size]

    for index in range(section_count):
        header = section_header(index)
        name_offset = _read_u32(header, 0)
        name_end = section_names.find(b"\x00", name_offset)
        if name_end == -1:
            raise ValueError(f"{elf} section name table is truncated")
        name = section_names[name_offset:name_end].decode("ascii")
        if name == section_name:
            return _read_u32(header, 12), _read_u32(header, 16), _read_u32(header, 20)

    raise ValueError(f"{elf} does not contain section {section_name}")


def _elf_flash_payload_len(elf: Path, payload_address: int) -> int:
    data = elf.read_bytes()
    _validate_elf32_le(data, elf)

    program_header_offset = _read_u32(data, 28)
    program_header_size = _read_u16(data, 42)
    program_count = _read_u16(data, 44)
    if program_header_size < 32:
        raise ValueError(f"{elf} has invalid program header size")

    payload_end = payload_address
    for index in range(program_count):
        start = program_header_offset + index * program_header_size
        end = start + program_header_size
        if end > len(data):
            raise ValueError(f"{elf} program header table is truncated")
        header = data[start:end]
        segment_type = _read_u32(header, 0)
        segment_address = _read_u32(header, 12)
        segment_file_size = _read_u32(header, 16)
        if segment_type != 1 or segment_file_size == 0:
            continue
        # RAM LOAD segments must not turn the image into a sparse flash range.
        if (segment_address ^ payload_address) & 0xFF00_0000:
            continue
        segment_end = segment_address + segment_file_size
        if segment_end > payload_address:
            payload_end = max(payload_end, segment_end)

    payload_len = payload_end - payload_address
    if payload_len == 0:
        raise ValueError(f"{elf} does not contain a flash payload after .image_header")
    return payload_len


def cmd_render(args: argparse.Namespace, code_size: int) -> None:
    manifest = _load_header_manifest(args.manifest)
    header = build_header(
        manifest=manifest,
        marketing_version=_marketing_version(args.versions_manifest, args.version_key),
        monotonic_version=args.monotonic_version,
        code_size=code_size,
    )
    args.output.write_bytes(header)


def cmd_finalize_code_size(args: argparse.Namespace) -> None:
    header = args.header.read_bytes()
    payload = args.payload.read_bytes()
    args.output.write_bytes(finalize_code_size(header, payload))


def cmd_finalize_elf(args: argparse.Namespace) -> None:
    elf = args.elf
    header_address, header_offset, header_size = _elf_section(elf, ".image_header")
    if header_size != HEADER_LEN:
        raise ValueError(
            f"{elf} .image_header must be exactly {HEADER_LEN} bytes, got {header_size}"
        )
    payload_len = _elf_flash_payload_len(elf, header_address + header_size)

    with elf.open("r+b") as outfile:
        outfile.seek(header_offset)
        header_bytes = outfile.read(header_size)
        outfile.seek(header_offset)
        outfile.write(finalize_header_code_size(header_bytes, payload_len))

    print(f"finalized {elf}: code_size={payload_len}")


def build_parser() -> argparse.ArgumentParser:
    """Build the command-line argument parser."""
    parser = argparse.ArgumentParser(description=__doc__)
    subparsers = parser.add_subparsers(dest="command", required=True)

    for command in ("render-dev-header", "render-release-header"):
        subparser = subparsers.add_parser(command)
        subparser.add_argument("--manifest", type=Path, required=True)
        subparser.add_argument("--versions-manifest", type=Path, required=True)
        subparser.add_argument("--version-key", choices=("bootloader", "firmware"), required=True)
        subparser.add_argument("--monotonic-version", type=int, required=True)
        subparser.add_argument("--output", type=Path, required=True)

    finalize = subparsers.add_parser("finalize-code-size")
    finalize.add_argument("--header", type=Path, required=True)
    finalize.add_argument("--payload", type=Path, required=True)
    finalize.add_argument("--output", type=Path, required=True)

    finalize_elf = subparsers.add_parser("finalize-elf")
    finalize_elf.add_argument("elf", type=Path)
    return parser


def main() -> int:
    args = build_parser().parse_args()
    if args.command == "render-dev-header":
        cmd_render(args, INVALID_CODE_SIZE)
    elif args.command == "render-release-header":
        cmd_render(args, 0)
    elif args.command == "finalize-code-size":
        cmd_finalize_code_size(args)
    else:
        cmd_finalize_elf(args)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
