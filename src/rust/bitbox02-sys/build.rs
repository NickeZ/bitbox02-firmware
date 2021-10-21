use std::process::Command;
use std::fs::File;
use std::io::prelude::*;
use temp_dir::TempDir;

fn main() {
    let path_to_bindings = if let Ok(cmake_dir) = std::env::var("CMAKE_CURRENT_BINARY_DIR") {
        // if we are being invoked from CMAKE, the bindings are here:
        format!("{}/rust", cmake_dir)
    } else {
        let bitbox02_sys_dir = std::env::var("CARGO_MANIFEST_DIR").unwrap();
        let cmake_dir = format!("{}/../../../", bitbox02_sys_dir);

        // generate list of includes using CMake
        let tempdir = TempDir::with_prefix("bitbox02").unwrap();
        let tempdir = tempdir.path().to_str().unwrap();
        let _ = Command::new("cmake").arg(&cmake_dir).current_dir(&tempdir).output().unwrap();
        let _ = Command::new("make").arg("rust-bindgen-includes").current_dir(&tempdir).output().unwrap();
        let mut includes_file = File::open(format!("{}/src/rust-bindgen.flags", tempdir)).unwrap();
        let mut includes = String::new();
        includes_file.read_to_string(&mut includes).unwrap();
        let includes:Vec<&str> = includes.trim().split_ascii_whitespace().collect();
        let target = std::env::var("TARGET").unwrap();
        let mut flags = vec!["-target", &target, "-DTESTING=1"];
        flags.extend(&includes);

        // generate bindings
        let outdir = std::env::var("OUT_DIR").unwrap();
        let generate_bindings = format!("{}/scripts/generate-bindings.sh", cmake_dir);
        let bindings = format!("{}/bindings.rs", outdir);
        let wrapper = format!("{}/wrapper.h", bitbox02_sys_dir);
        let _ = Command::new(&generate_bindings)
            .args(&[&bindings, &wrapper])
            .args(&flags)
            .output()
            .unwrap();

        outdir
    };
    println!("cargo:rustc-env=BINDINGS={}/bindings.rs", path_to_bindings);
}
