use std::path::{Path, PathBuf};

fn build_board_hal_overrides_object(repo_root: &Path, out_dir: &Path) {
    #[cfg(feature = "board-stm32u5-dk")]
    bitbox_board_stm32u5_dk_build::build_hal_overrides_object(repo_root, out_dir);
    #[cfg(not(feature = "board-stm32u5-dk"))]
    let _ = (repo_root, out_dir);
}

fn main() {
    if !cfg!(feature = "board-stm32u5-dk") {
        panic!("select a BitBox03 board feature, e.g. `board-stm32u5-dk`")
    }

    let target = std::env::var("TARGET").expect("TARGET not set");
    if target.starts_with("thumb") {
        let manifest_dir =
            PathBuf::from(std::env::var("CARGO_MANIFEST_DIR").expect("CARGO_MANIFEST_DIR not set"));
        let out_dir = PathBuf::from(std::env::var("OUT_DIR").expect("OUT_DIR not set"));
        let repo_root = manifest_dir.join("../../../..");

        let lds_from = manifest_dir.join("bitbox03-boot0.ld");
        let lds_to = out_dir.join("bitbox03-boot0.ld");
        println!("cargo::rerun-if-changed={}", lds_from.display());
        std::fs::copy(lds_from, &lds_to).expect("copy linker script");

        println!("cargo::rustc-link-search={}", out_dir.display());
        println!(
            "cargo::rustc-link-arg=-Map={}",
            out_dir.join("bitbox03-boot0.map").display()
        );
        println!("cargo::rustc-link-arg=-Tbitbox03-boot0.ld");

        build_board_hal_overrides_object(&repo_root, &out_dir);
    }
}
