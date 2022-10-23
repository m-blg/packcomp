#pragma once

#include "mbgldef.h"
#include "basic.cc"
#include "array.cc"
#include "memory.cc"
#include <cassert>

namespace cp {



constexpr size_t bitfld_byte_count(size_t t_bit_count) {
    size_t div = t_bit_count / 8u;
    size_t mod = t_bit_count % 8u;
    return (mod ? div + 1 : div);
}

inline bool bitfld_get_bit(u8* buffer, size_t bit_index) {
    return buffer[bit_index / 8u] & (1u << (7u - bit_index % 8u));
}

inline void bitfld_set_bit_high(u8* buffer, size_t bit_index) {
    buffer[bit_index / 8u] |= (1u << (7u - bit_index % 8u)); 
}

inline void bitfld_set_bit_low(u8* buffer, size_t bit_index) {
    buffer[bit_index / 8u] &= ~(1u << (7u - bit_index % 8u)); 
}

inline void bitfld_flip_bit(u8* buffer, size_t bit_index) {
    buffer[bit_index / 8u] ^= (1u << (7u - bit_index % 8u)); 
}

inline void bitfld_set_bit(u8* buffer, size_t bit_index, bool value) {
    if (bitfld_get_bit(buffer, bit_index) != value) {
        bitfld_flip_bit(buffer, bit_index);
    }
}

inline int bitfld_cmp_bits(u8* first, u8* second, size_t bit_count) {
    for (size_t i = 0; i < bit_count; i++) {
        if (bitfld_get_bit(first, i) != bitfld_get_bit(second, i)) {
            return (bitfld_get_bit(first, i) ? 1 : -1);
        }
    }
    return 0;
}

inline void bitfld_print_bits(u8* buffer, size_t bit_count) {
    for (i32 i = 0; i < bit_count; i++) {
        printf("%d", bitfld_get_bit(buffer, i));
    }
}
    

template <size_t t_bit_count>
struct Static_Bit_Field {
    u8 buffer[bitfld_byte_count(t_bit_count)];

    constexpr size_t bit_count() { return t_bit_count; }
    // in bytes
    constexpr size_t cap() { return bitfld_byte_count(t_bit_count); }
};

template <size_t t_bit_count>
using sbitfld = Static_Bit_Field<t_bit_count>;

template <size_t t_bit_count>
void clear(sbitfld<t_bit_count> *self, i32 value=0) {
    memset(self->buffer, value, self->cap());
}

template <size_t t_bit_count>
bool get_bit(sbitfld<t_bit_count> *self, size_t bit_index) {
    assert(("Bit index out of range", bit_index < t_bit_count));
    return bitfld_get_bit(self->buffer, bit_index);
}

template <size_t t_bit_count>
void set_bit_high(sbitfld<t_bit_count> *self, size_t bit_index) {
    assert(("Bit index out of range", bit_index < t_bit_count));
    return bitfld_set_bit_high(self->buffer, bit_index);
}

template <size_t t_bit_count>
void set_bit_low(sbitfld<t_bit_count> *self, size_t bit_index) {
    assert(("Bit index out of range", bit_index < t_bit_count));
    return bitfld_set_bit_low(self->buffer, bit_index);
}

template <size_t t_bit_count>
void flip_bit(sbitfld<t_bit_count> *self, size_t bit_index) {
    assert(("Bit index out of range", bit_index < t_bit_count));
    return bitfld_flip_bit(self->buffer, bit_index);
}

template <size_t t_bit_count>
void set_bit(sbitfld<t_bit_count> *self, size_t bit_index, bool value) {
    assert(("Bit index out of range", bit_index < t_bit_count));
    return bitfld_set_bit(self->buffer, bit_index, value);
}


struct Dynamic_Bit_Field {
    u8* buffer;
    size_t cap; // in bytes
    size_t bit_count;


    static i32 cmp_bitfld(Dynamic_Bit_Field *first, Dynamic_Bit_Field *second) {
        if (first->bit_count == second->bit_count) {
            return bitfld_cmp_bits(first->buffer, second->buffer, first->bit_count);
        } else {
            return (first->bit_count > second->bit_count ? 1 : -1);
        }
    }

    bool operator==(Dynamic_Bit_Field& other) {
        return (cmp_bitfld(this, &other) == 0);
    }

};

using dbitfld = Dynamic_Bit_Field;


void init_bits(dbitfld *self, size_t init_bit_count=0) { 
    self->cap = bitfld_byte_count(init_bit_count); 
    self->bit_count = init_bit_count;
    self->buffer = m_alloc<u8>(self->cap); 
}
void init_bytes(dbitfld *self, size_t init_cap=0) { 
    self->cap = init_cap; 
    self->bit_count = init_cap * 8u;
    self->buffer = m_alloc<u8>(init_cap); 
}
void shut(dbitfld *self) { free(self->buffer); }


inline size_t cap(dbitfld *buffer) { return buffer->cap; }
inline u8* begin(dbitfld *buffer) { return buffer->buffer; }
inline u8* end(dbitfld *buffer) { return buffer->buffer + buffer->cap; }

void clear(dbitfld *self, i32 value=0) {
    memset(self->buffer, value, self->cap);
}

bool get_bit(dbitfld self, size_t bit_index) {
    //assert(("Bit index out of range", bit_index < bit_count));
    return bitfld_get_bit(self.buffer, bit_index);
}

void set_bit_high(dbitfld self, size_t bit_index) {
    //assert(("Bit index out of range", bit_index < bit_count));
    return bitfld_set_bit_high(self.buffer, bit_index);
}

void set_bit_low(dbitfld self, size_t bit_index) {
    //assert(("Bit index out of range", bit_index < bit_count));
    return bitfld_set_bit_low(self.buffer, bit_index);
}

void flip_bit(dbitfld self, size_t bit_index) {
    //assert(("Bit index out of range", bit_index < bit_count));
    return bitfld_flip_bit(self.buffer, bit_index);
}

void set_bit(dbitfld self, size_t bit_index, bool value) {
    //assert(("Bit index out of range", bit_index < bit_count));
    return bitfld_set_bit(self.buffer, bit_index, value);
}

void dpush_bit(dbitfld *self, bool value) {
    size_t len = (self->bit_count + 1u) / 8u + (size_t)((self->bit_count + 1u) % 8u > 0);
    if (len >= self->cap) {
        resize( &self->buffer, &self->cap, max(1u, 2u * ((*self).cap)) );
    }
    set_bit(*self, self->bit_count, value);
    self->bit_count++;
}

void print_bits(dbitfld self) {
    bitfld_print_bits(self.buffer, self.bit_count);
}

}
