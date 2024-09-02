// bool salt_hash_data(const uint8_t* data, size_t data_len, const char* purpose, uint8_t* hash_out);
pub fn hash_data(data: &[u8], purpose: &str) -> Result<[u8; 32], &'static str> {
    let mut hash = [0; 32];
    if ! unsafe {bitbox02_sys::salt_hash_data(data.as_ptr() as _, data.len(), purpose.as_ptr(), &mut hash as *mut _) } {
        return Err("Could not fetch salt root");
    }
    Ok(hash)
}
