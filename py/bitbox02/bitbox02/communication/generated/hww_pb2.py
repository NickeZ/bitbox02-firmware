# -*- coding: utf-8 -*-
# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: hww.proto
"""Generated protocol buffer code."""
from google.protobuf.internal import builder as _builder
from google.protobuf import descriptor as _descriptor
from google.protobuf import descriptor_pool as _descriptor_pool
from google.protobuf import symbol_database as _symbol_database
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()


from . import common_pb2 as common__pb2
from . import backup_commands_pb2 as backup__commands__pb2
from . import bitbox02_system_pb2 as bitbox02__system__pb2
from . import bluetooth_pb2 as bluetooth__pb2
from . import btc_pb2 as btc__pb2
from . import cardano_pb2 as cardano__pb2
from . import eth_pb2 as eth__pb2
from . import keystore_pb2 as keystore__pb2
from . import mnemonic_pb2 as mnemonic__pb2
from . import system_pb2 as system__pb2
from . import perform_attestation_pb2 as perform__attestation__pb2


DESCRIPTOR = _descriptor_pool.Default().AddSerializedFile(b'\n\thww.proto\x12\x14shiftcrypto.bitbox02\x1a\x0c\x63ommon.proto\x1a\x15\x62\x61\x63kup_commands.proto\x1a\x15\x62itbox02_system.proto\x1a\x0f\x62luetooth.proto\x1a\tbtc.proto\x1a\rcardano.proto\x1a\teth.proto\x1a\x0ekeystore.proto\x1a\x0emnemonic.proto\x1a\x0csystem.proto\x1a\x19perform_attestation.proto\"&\n\x05\x45rror\x12\x0c\n\x04\x63ode\x18\x01 \x01(\x05\x12\x0f\n\x07message\x18\x02 \x01(\t\"\t\n\x07Success\"\xba\x0e\n\x07Request\x12\x41\n\x0b\x64\x65vice_name\x18\x02 \x01(\x0b\x32*.shiftcrypto.bitbox02.SetDeviceNameRequestH\x00\x12I\n\x0f\x64\x65vice_language\x18\x03 \x01(\x0b\x32..shiftcrypto.bitbox02.SetDeviceLanguageRequestH\x00\x12>\n\x0b\x64\x65vice_info\x18\x04 \x01(\x0b\x32\'.shiftcrypto.bitbox02.DeviceInfoRequestH\x00\x12@\n\x0cset_password\x18\x05 \x01(\x0b\x32(.shiftcrypto.bitbox02.SetPasswordRequestH\x00\x12\x42\n\rcreate_backup\x18\x06 \x01(\x0b\x32).shiftcrypto.bitbox02.CreateBackupRequestH\x00\x12\x42\n\rshow_mnemonic\x18\x07 \x01(\x0b\x32).shiftcrypto.bitbox02.ShowMnemonicRequestH\x00\x12\x36\n\x07\x62tc_pub\x18\x08 \x01(\x0b\x32#.shiftcrypto.bitbox02.BTCPubRequestH\x00\x12\x41\n\rbtc_sign_init\x18\t \x01(\x0b\x32(.shiftcrypto.bitbox02.BTCSignInitRequestH\x00\x12\x43\n\x0e\x62tc_sign_input\x18\n \x01(\x0b\x32).shiftcrypto.bitbox02.BTCSignInputRequestH\x00\x12\x45\n\x0f\x62tc_sign_output\x18\x0b \x01(\x0b\x32*.shiftcrypto.bitbox02.BTCSignOutputRequestH\x00\x12O\n\x14insert_remove_sdcard\x18\x0c \x01(\x0b\x32/.shiftcrypto.bitbox02.InsertRemoveSDCardRequestH\x00\x12@\n\x0c\x63heck_sdcard\x18\r \x01(\x0b\x32(.shiftcrypto.bitbox02.CheckSDCardRequestH\x00\x12\x64\n\x1fset_mnemonic_passphrase_enabled\x18\x0e \x01(\x0b\x32\x39.shiftcrypto.bitbox02.SetMnemonicPassphraseEnabledRequestH\x00\x12@\n\x0clist_backups\x18\x0f \x01(\x0b\x32(.shiftcrypto.bitbox02.ListBackupsRequestH\x00\x12\x44\n\x0erestore_backup\x18\x10 \x01(\x0b\x32*.shiftcrypto.bitbox02.RestoreBackupRequestH\x00\x12N\n\x13perform_attestation\x18\x11 \x01(\x0b\x32/.shiftcrypto.bitbox02.PerformAttestationRequestH\x00\x12\x35\n\x06reboot\x18\x12 \x01(\x0b\x32#.shiftcrypto.bitbox02.RebootRequestH\x00\x12@\n\x0c\x63heck_backup\x18\x13 \x01(\x0b\x32(.shiftcrypto.bitbox02.CheckBackupRequestH\x00\x12/\n\x03\x65th\x18\x14 \x01(\x0b\x32 .shiftcrypto.bitbox02.ETHRequestH\x00\x12\x33\n\x05reset\x18\x15 \x01(\x0b\x32\".shiftcrypto.bitbox02.ResetRequestH\x00\x12Q\n\x15restore_from_mnemonic\x18\x16 \x01(\x0b\x32\x30.shiftcrypto.bitbox02.RestoreFromMnemonicRequestH\x00\x12\x43\n\x0b\x66ingerprint\x18\x18 \x01(\x0b\x32,.shiftcrypto.bitbox02.RootFingerprintRequestH\x00\x12/\n\x03\x62tc\x18\x19 \x01(\x0b\x32 .shiftcrypto.bitbox02.BTCRequestH\x00\x12U\n\x17\x65lectrum_encryption_key\x18\x1a \x01(\x0b\x32\x32.shiftcrypto.bitbox02.ElectrumEncryptionKeyRequestH\x00\x12\x37\n\x07\x63\x61rdano\x18\x1b \x01(\x0b\x32$.shiftcrypto.bitbox02.CardanoRequestH\x00\x12\x33\n\x05\x62ip85\x18\x1c \x01(\x0b\x32\".shiftcrypto.bitbox02.BIP85RequestH\x00\x12;\n\tbluetooth\x18\x1d \x01(\x0b\x32&.shiftcrypto.bitbox02.BluetoothRequestH\x00\x42\t\n\x07requestJ\x04\x08\x01\x10\x02J\x04\x08\x17\x10\x18\"\xfd\x07\n\x08Response\x12\x30\n\x07success\x18\x01 \x01(\x0b\x32\x1d.shiftcrypto.bitbox02.SuccessH\x00\x12,\n\x05\x65rror\x18\x02 \x01(\x0b\x32\x1b.shiftcrypto.bitbox02.ErrorH\x00\x12?\n\x0b\x64\x65vice_info\x18\x04 \x01(\x0b\x32(.shiftcrypto.bitbox02.DeviceInfoResponseH\x00\x12\x30\n\x03pub\x18\x05 \x01(\x0b\x32!.shiftcrypto.bitbox02.PubResponseH\x00\x12\x42\n\rbtc_sign_next\x18\x06 \x01(\x0b\x32).shiftcrypto.bitbox02.BTCSignNextResponseH\x00\x12\x41\n\x0clist_backups\x18\x07 \x01(\x0b\x32).shiftcrypto.bitbox02.ListBackupsResponseH\x00\x12\x41\n\x0c\x63heck_backup\x18\x08 \x01(\x0b\x32).shiftcrypto.bitbox02.CheckBackupResponseH\x00\x12O\n\x13perform_attestation\x18\t \x01(\x0b\x32\x30.shiftcrypto.bitbox02.PerformAttestationResponseH\x00\x12\x41\n\x0c\x63heck_sdcard\x18\n \x01(\x0b\x32).shiftcrypto.bitbox02.CheckSDCardResponseH\x00\x12\x30\n\x03\x65th\x18\x0b \x01(\x0b\x32!.shiftcrypto.bitbox02.ETHResponseH\x00\x12\x44\n\x0b\x66ingerprint\x18\x0c \x01(\x0b\x32-.shiftcrypto.bitbox02.RootFingerprintResponseH\x00\x12\x30\n\x03\x62tc\x18\r \x01(\x0b\x32!.shiftcrypto.bitbox02.BTCResponseH\x00\x12V\n\x17\x65lectrum_encryption_key\x18\x0e \x01(\x0b\x32\x33.shiftcrypto.bitbox02.ElectrumEncryptionKeyResponseH\x00\x12\x38\n\x07\x63\x61rdano\x18\x0f \x01(\x0b\x32%.shiftcrypto.bitbox02.CardanoResponseH\x00\x12\x34\n\x05\x62ip85\x18\x10 \x01(\x0b\x32#.shiftcrypto.bitbox02.BIP85ResponseH\x00\x12<\n\tbluetooth\x18\x11 \x01(\x0b\x32\'.shiftcrypto.bitbox02.BluetoothResponseH\x00\x42\n\n\x08responseJ\x04\x08\x03\x10\x04\x62\x06proto3')

_builder.BuildMessageAndEnumDescriptors(DESCRIPTOR, globals())
_builder.BuildTopDescriptorsAndMessages(DESCRIPTOR, 'hww_pb2', globals())
if _descriptor._USE_C_DESCRIPTORS == False:

  DESCRIPTOR._options = None
  _ERROR._serialized_start=222
  _ERROR._serialized_end=260
  _SUCCESS._serialized_start=262
  _SUCCESS._serialized_end=271
  _REQUEST._serialized_start=274
  _REQUEST._serialized_end=2124
  _RESPONSE._serialized_start=2127
  _RESPONSE._serialized_end=3148
# @@protoc_insertion_point(module_scope)
