#include <string.h>
#include <stylus_types.h>
#include <bebi.h>
#include <erc20/ERC20.h>
#include <storage.h>
#include <stdbool.h>
#include <stylus_debug.h>
#include <stylus_utils.h>

uint8_t buf_out[32];

ArbResult inline _success_bebi32(bebi32 const retval) {
    ArbResult res = {Success, retval , 32};
    return res;
}

void inline load_shorts(const void *storage, uint64_t* minters_entries_out, uint64_t *minters_current_out, bool *initialized_out) {
    bebi32 storage_slot = STORAGE_SLOT_minters_entries;
    bebi32 buf;
    storage_load(storage, storage_slot, buf);
    if (minters_entries_out != NULL) {
        uint64_t res = bebi_get_u64(buf, STORAGE_OFFSET_minters_entries);
        *minters_entries_out = res;
    }
    if (minters_current_out != NULL) {
        uint64_t res = bebi_get_u64(buf, STORAGE_OFFSET_minters_current);
        *minters_current_out = res;
    }
    if (initialized_out != NULL) {
        uint8_t res = bebi_get_u8(buf, STORAGE_OFFSET_initialized);
        *initialized_out = (res != 0);
    }
}

// always initialized==1 when storing
void inline store_shorts(void *storage, uint64_t minters_entries_out, uint64_t minters_current_out) {
    bebi32 storage_slot = STORAGE_SLOT_minters_entries;
    bebi32 buf;
    bebi_set_u64(buf, STORAGE_OFFSET_minters_entries, minters_entries_out);
    bebi_set_u64(buf, STORAGE_OFFSET_minters_current, minters_current_out);
    bebi_set_u8(buf, STORAGE_OFFSET_initialized, 1);
    storage_store(storage, storage_slot, buf);
}

void minter_idx_storage_slot(bebi32 const minter, bebi32 slot_out) {
    bebi32 base = STORAGE_SLOT_minter_idx;
    map_slot(base, minter, 32, slot_out);
}

void balance_slot(bebi32 const account, bebi32 slot_out) {
    bebi32 base = STORAGE_SLOT__balances;
    map_slot(base, account, 32, slot_out);    
}

uint64_t _minter_idx(const void *storage, bebi32 const minter) {
    bebi32 slot;
    minter_idx_storage_slot(minter, slot);
    bebi32 buffer;
    storage_load(storage, slot, buffer);
    return bebi32_get_u64(buffer);
}

bool from_minter(const void *storage) {
    bebi32 sender;
    msg_sender_padded(sender);
    return  _minter_idx(storage, sender) != 0;
}

void add_minters_in_idx(void *storage, bebi32 minter, uint64_t index) {
    bebi32 idx_slot;
    minter_idx_storage_slot(minter, idx_slot);
    bebi32 idx_bebi;
    bebi32_set_u64(idx_bebi, index);
    storage_store(storage, idx_slot, idx_bebi);

    bebi32 minters_slot;
    bebi32 minters_base = STORAGE_SLOT_minters;
    array_slot_offset(minters_base, 32, index, minters_slot, NULL);
    storage_store(storage, minters_slot, minter);
}

ArbResult default_func(void *storage, uint8_t *input, size_t len, bebi32 value) {
    log_i64(bebi32_get_u64(value));
    return _return_short_string(Failure, "not supported");
}

ArbResult minters(const void *storage, uint8_t *input, size_t len) { // minters(uint256)
    if (len != 32 || !bebi32_is_u64(input)) {
        return _return_short_string(Failure, "symbol: length");
    }
    uint64_t index = bebi32_get_u64(input);
    bebi32 storage_slot = STORAGE_SLOT_minters;
    bebi32_add_u64(storage_slot, index);
    storage_load(storage, storage_slot, buf_out);
    return _success_bebi32(buf_out);
}

ArbResult symbol(uint8_t *input, size_t len) { // symbol()
    if (len != 0) {
        return _return_short_string(Failure, "symbol: length");
    }
    return _return_short_string(Success, "CTOK");
}

ArbResult minters_current(const void *storage, uint8_t *input, size_t len) { // minters_current()
    uint64_t minters_current;
    load_shorts(storage, NULL, &minters_current, NULL);
    bebi32_set_u64(buf_out, minters_current);
    return _success_bebi32(buf_out);
}

ArbResult remove_minter(void *storage, uint8_t *input, size_t len) { // remove_minter(address)
    if (!from_minter(storage)) {
        return _return_short_string(Failure, "remove: not from minter");
    }
    if (len != 32 || !bebi32_is_u160(input)) {
        return _return_short_string(Failure, "remove: input");
    }
    // if we are from minter this must already be enabled
    bebi32 idx_slot;
    bebi32 buf;
    minter_idx_storage_slot(input, idx_slot);
    storage_load(storage, idx_slot, buf);
    uint64_t idx = bebi32_get_u32(buf);
    if (!idx) {
        return  _return_short_string(Failure, "remove: not minter");
    }
    bebi32 minters_slot;
    bebi32 minters_base = STORAGE_SLOT_minters;
    array_slot_offset(minters_base, 32, idx, minters_slot, NULL);
    uint64_t minters_entries, minters_current;
    load_shorts(storage, &minters_entries, &minters_current, NULL);
    bebi32 zero;
    bebi32_set_u64(zero, 0);
    storage_store(storage, idx_slot, zero);
    storage_store(storage, minters_slot, zero);
    store_shorts(storage, minters_entries, minters_current-1);
    return _return_nodata(Success);
}

ArbResult add_minter(void *storage, uint8_t *input, size_t len) { // add_minter(address)
    if (!from_minter(storage)) {
        return _return_nodata(Failure);
    }
    if (len != 32 || !bebi32_is_u160(input)) {
        return _return_nodata(Failure);
    }
    uint64_t minters_entries, minters_current;
    load_shorts(storage, &minters_entries, &minters_current, NULL);
    minters_current += 1;
    minters_entries +=1;

    add_minters_in_idx(storage, input, minters_entries);

    store_shorts(storage, minters_entries, minters_current);
    return _return_nodata(Success);
}

ArbResult mint(void *storage, uint8_t *input, size_t len){ // mint(address,uint256)
    if (!from_minter(storage)) {
        return _return_nodata(Failure);
    }
    if (len != 64) {
        return _return_nodata(Failure);
    }
    uint8_t const *dest = input;
    uint8_t const *amount = (input + 32);
    if (!bebi32_is_u160(dest)) {
        return _return_nodata(Failure);
    }
    bebi32 balance_slot_buf;
    balance_slot(dest, balance_slot_buf);
    bebi32 balance_buf;
    storage_load(storage, balance_slot_buf, balance_buf);

    int overflow = bebi32_add(balance_buf, amount);
    if (overflow) {
        return _return_nodata(Failure);
    }

    bebi32 total_supply_slot = STORAGE_SLOT__totalSupply;

    bebi32 total_supply;

    storage_load(storage, total_supply_slot, total_supply);
    overflow = bebi32_add(total_supply, amount);
    if (overflow) {
        return _return_nodata(Failure);
    }
    storage_store(storage, total_supply_slot, total_supply);
    storage_store(storage, balance_slot_buf, balance_buf);
    return _return_nodata(Success);
}

ArbResult name(uint8_t *input, size_t len){ // name()
    return symbol(input, len);
}

ArbResult minters_entries(const void *storage, uint8_t *input, size_t len){ // minters_entries()
    uint64_t minters_entries;
    load_shorts(storage, &minters_entries, NULL, NULL);
    bebi32_set_u64(buf_out, minters_entries);
    return _success_bebi32(buf_out);
}

ArbResult transfer(void *storage, uint8_t *input, size_t len) { // transfer(address,uint256)
    if (len != 64) {
        return _return_nodata(Failure);
    }
    uint8_t const *dest = input;
    uint8_t const *amount = (input + 32);
    if (!bebi32_is_u160(dest)) {
        return _return_nodata(Failure);
    }
    log_i32(11);
    bebi32 sender;
    msg_sender_padded(sender);
    bebi32 balance_slot_buf;
    balance_slot(sender, balance_slot_buf);

    bebi32 balance_buf;
    storage_load(storage, balance_slot_buf, balance_buf);
    if (bebi32_cmp(balance_buf, amount) < 0) {
        return _return_short_string(Failure, "balance too low");
    }
    bebi32_sub(balance_buf, amount);
    storage_store(storage, balance_slot_buf, balance_buf);

    balance_slot(dest, balance_slot_buf);
    storage_load(storage, balance_slot_buf, balance_buf);
    int overflow = bebi32_add(balance_buf, amount);
    if (overflow) {
        return _return_short_string(Failure, "reciever overflow");
    }
    storage_store(storage, balance_slot_buf, balance_buf);

    return _return_nodata(Success);
}

ArbResult minter_idx(const void *storage, uint8_t *input, size_t len) { // minter_idx(address)
    if (len != 32 || !bebi32_is_u160(input)) {
        return _return_nodata(Failure);
    }
    uint64_t idx = _minter_idx(storage, input);
    bebi32_set_u64(buf_out, idx);
    return _success_bebi32(buf_out);
}

ArbResult balanceOf(const void *storage, uint8_t *input, size_t len) { // balanceOf(address)
    if (len != 32) {
        return _return_short_string(Failure, "balance: input len");
    }
    if (!bebi32_is_u160(input)) {
        return _return_short_string(Failure, "balance: not address");
    }
    bebi32 slot;
    balance_slot(input, slot);
    storage_load(storage, slot, buf_out);
    return _success_bebi32(buf_out);    
}

ArbResult init(void *storage, uint8_t *input, size_t len) { // init()
    log_i32(900000000);
    if (len != 32 || !bebi32_is_u160(input)) {
        return _return_short_string(Failure, "init: input");
    }
    bool initialized;
    load_shorts(storage, NULL, NULL, &initialized);
    if (initialized) {
        return _return_short_string(Failure, "init: initialized");
    }
    add_minters_in_idx(storage, input, 1);
    store_shorts(storage, 1, 1);
    return _return_nodata(Success);
}

ArbResult decimals(uint8_t *input, size_t len) { // decimals()
    bebi32_set_u16(buf_out, 18);
    return _success_bebi32(buf_out);
}

ArbResult totalSupply(const void *storage, uint8_t *input, size_t len){ // totalSupply()
    bebi32 total_supply_slot = STORAGE_SLOT__totalSupply;

    storage_load(storage, total_supply_slot, buf_out);
    return _success_bebi32(buf_out);
}

