#pragma once

#include "mbgldef.h"
#include "basic.cc"
#include "buffer.cc"
#include <cassert>
#include <string.h>


namespace cp {


template <typename T>
void arr_rpush(T* buffer, size_t *len, T item) { // push with value
    buffer[(*len)] = item;
    (*len)++;
}
    
template <typename T>
void arr_rpop(T* buffer, size_t *len) {
    assert(*len > 0);
    (*len)--;
    return buffer[(*len)];
}

template <typename T>
void arr_add(T* buffer, size_t *len, T item, size_t index) { // add with value
    assert(index < *len);
    T* p_target = buffer + index;
    for (T* p = buffer + (*len); p > p_target; p--) {
        *p = *(p - 1);
    }
    *p_target = item;
    (*len)++;
}

template <typename T>
void arr_remove(T* buffer, size_t *len, size_t index) {
    assert(*len > 0);
    (*len)--;
    T* p_end = buffer + (*len);
    for (T* p = buffer + index; p < p_end; p++) {
        *p = *(p + 1);
    }
}

template <typename T, bool t_check_null=true>
void arr_remove(T* buffer, size_t *len, T* it) {
    assert(*len > 0);
    if constexpr(t_check_null) {
        assert(it != null);
    }
    (*len)--;
    T* p_end = buffer + (*len);
    for (T* p = it; p < p_end; p++) {
        *p = *(p + 1);
    }
}


void arr_rprint(i8* buffer, size_t stride, size_t len, const char* item_fmt) {
    i8* endp = buffer + stride * len;
    for (i8* p = buffer; p < endp; p += stride) {
        printf(item_fmt, *p);
    }
}



template <typename T, size_t t_cap>
struct Static_Array {
    T buffer[t_cap];
    size_t len;
    
    // explicit Static_Array() = default;
    // Static_Array(size_t _len=0)
    // : len(_len) {}
    
    T& operator[](size_t index) {
        assert(("Index out of range", index < t_cap));
        return buffer[index];
    }
   
    typedef T type;
    typedef buff_iter<T> iter;
};

template <typename T, size_t t_cap>
using sarr = Static_Array<T, t_cap>;

template <size_t t_cap>
using sarru = Static_Array<size_t, t_cap>;
template <size_t t_cap>
using sarri = Static_Array<i32, t_cap>;
template <size_t t_cap>
using sarrf = Static_Array<f32, t_cap>;
template <size_t t_cap>
using sarrd = Static_Array<f64, t_cap>;

template <typename T, size_t t_cap>
inline void init(sarr<T, t_cap> *self, size_t init_len=0) {
    assert(init_len <= t_cap);
    self->len = init_len; 
};

template <typename T, size_t t_cap>
inline size_t cap(sarr<T, t_cap> &self) { return t_cap; }
template <typename T, size_t t_cap>
inline size_t len(sarr<T, t_cap> &self) { return self.len; }
template <typename T, size_t t_cap>
inline T* beginp(sarr<T, t_cap> &self) { return self.buffer; }
template <typename T, size_t t_cap>
inline T* endp(sarr<T, t_cap> &self) { return self.buffer + self.len; }
template <typename T, size_t t_cap>
inline buff_iter<T> begin(sarr<T, t_cap> &self) { return {self.buffer}; }
template <typename T, size_t t_cap>
inline buff_iter<T> end(sarr<T, t_cap> &self) { return {self.buffer + self.len}; }

template <typename T, size_t t_cap>
inline bool is_empty(sarr<T, t_cap> &self) { return (self.len == 0); }

template <typename T, size_t t_cap>
inline T& back(sarr<T, t_cap> &self) { return self.buffer[self.len-1]; }



template <typename T, size_t t_cap>
void push(sarr<T, t_cap> *self, T item) {
    assert(self->len < t_cap);
    arr_rpush(self->buffer, &self->len, item);
}

template <typename T, size_t t_cap>
T pop(sarr<T, t_cap> *self) {
    assert(self->len > 0);
    return arr_pop(self->buffer, &self->len);
}

template <typename T, size_t t_cap>
void add(sarr<T, t_cap> *self, T item, size_t index) {
    arr_add(self->buffer, &self->len, item, index);
}

template <typename T, size_t t_cap>
void remove(sarr<T, t_cap> *self, size_t index) {
    arr_remove(self->buffer, &self->len, index);
}

template <typename T, size_t t_cap>
void print_fmt(sarr<T, t_cap> self, const char* item_fmt) {
    print_fmt(self.buffer, self.len, item_fmt);
}

template <typename T, size_t t_cap>
void print(sarr<T, t_cap> self) {
    print(self.buffer, self.len);
}



template <typename T>
struct Dynamic_Array {
    union {
        struct {
            T* buffer;
            size_t cap;
        };
        dbuff<T> _dbuff;
    };
    size_t len;

    T& operator[](size_t index) {
        assert(index < cap && "Index out of range");
        return buffer[index];
    }

    // template <size_t t_cap>
    // Dynamic_Array<T, t_cap> operator Dynamic_Array() {
    //     return {}
    // }

    typedef T type;
    typedef buff_iter<T> iter;
};

template <typename T>
using darr = Dynamic_Array<T>;

using darru = Dynamic_Array<size_t>;
using darri = Dynamic_Array<i32>;
using darrf = Dynamic_Array<f32>;
using darrd = Dynamic_Array<f64>;


template <typename T>
void init(darr<T> *self, size_t init_cap=0) { 
    self->buffer = m_alloc<T>(init_cap); 
    self->cap = init_cap; 
    self->len = 0; 
}

template <typename T, size_t t_arg_count>
void init_range(darr<T> *self, size_t init_cap, sbuff<T, t_arg_count> args) {
    self->buffer = m_alloc<T>(init_cap); 
    self->cap = init_cap;
    self->len = t_arg_count;
    memcpy(self->buffer->buffer, args.buffer, t_arg_count * sizeof(T));
}

template <typename T>
void shut(darr<T> *self) { 
    free(self->buffer); 
    *self = {};
}

template <typename T>
void shut(darr<const T> *self) { 
    *self = {};
}


template <typename T>
inline size_t cap(darr<T> self) { return self.cap; }
template <typename T>
inline size_t len(darr<T> self) { return self.len; }
template <typename T>
inline T* beginp(darr<T> self) { return self.buffer; }
template <typename T>
inline T* endp(darr<T> self) { return self.buffer + self.len; }
template <typename T>
inline buff_iter<T> begin(darr<T> self) { return {self.buffer}; }
template <typename T>
inline buff_iter<T> end(darr<T> self) { return {self.buffer + self.len}; }

template <typename T>
inline T& head(darr<T> self) {
    assert(len(self) > 0);
    return *self.buffer;
}
template <typename T>
inline darr<T> tail(darr<T> self) {
    assert(len(self) > 0);
    return {self.buffer + 1, self.cap, self.len - 1};
}

template <typename T>
inline bool is_empty(darr<T> self) { return (self.len == 0); }

template <typename T>
inline T& back(darr<T> self) { return self.buffer[self.len-1]; }


template <class T>
darr<T> copy(darr<T> a) {
    darr<T> na = {
        m_alloc<T>(len(a)),
        cap(a),
        len(a)
    };

    memcpy(na.buffer, a.buffer, sizeof(T) * len(a));

    return na;
}

// convert only sequence you abstracting, e.g. (a[0],..,a[len-1]) 
template <class T>
dbuff<T> to_dbuff(darr<T> a) {
    return {a.buffer, a.len};
}

template <class list_t>
auto to_darr(list_t l) {
    return darr<typename list_t::type>{beginp(l), len(l), len(l)};
}


//darr methods

template <typename T>
void fit_len(darr<T> *self) {
    if (self->len > self->cap) {
        resize(&self->_dbuff, max(self->len, 2 * self->cap));
    } else if (self->len < self->cap / 2) {
        resize(&self->_dbuff, self->cap / 2);
    }
}


template <typename T>
void shrink_to_fit(darr<T> *self) {
    resize(&self->_dbuff, self->len);
}

template <typename T>
void resize(darr<T> *self, size_t new_len) {
    self->len = new_len;
    fit_len(self);
}

template <typename T>
void reserve(darr<T> *self, size_t res_len) {
    size_t d = cap(*self) - len(*self);
    if (d < res_len) {
        size_t new_cap = max(len(*self) + res_len, 2 * cap(*self));
        resize(&self->_dbuff, new_cap);
    }
}

template <typename T>
void clear(darr<T> *self, i32 value=0) {
    resize(&self->_dbuff, 0);
    self->len = 0;
}

//raw push
template <typename T>
void rpush(darr<T> *self, T item) {
    arr_rpush(self->buffer, &self->len, item);
}

template <typename T>
void rpop(darr<T> *self) {
    arr_rpop(self->buffer, &self->len);
}

template <typename T>
void push(darr<T> *self, T item) {
    if (self->len >= self->cap) {
        resize(&self->buffer, &self->cap, 2 * self->cap + 1);
    }
    self->buffer[self->len] = item;
    self->len++;
}

template <typename T>
void pop(darr<T> *self) {
    assert(self->len > 0);
    if (self->len <= self->cap / 2u) {
        resize(&self->buffer, &self->cap, self->cap / 2);
    }
    self->len--;
}

template <typename T>
void add(darr<T> *self, T item, size_t index) {
    arr_add(self->buffer, &self->len, item, index);
}

template <typename T>
void remove(darr<T> *self, size_t index) {
    arr_remove(self->buffer, &self->len, index);
}

template <typename T>
void remove(darr<T> *self, T* it) {
    arr_remove(self->buffer, &self->len, it);
}

template <typename T>
void print_fmt(darr<T> *self, const char* item_fmt) {
    print_fmt(self->buffer, self->len, item_fmt);
}

template <typename T>
void print(darr<T> *self) {
    print(self->buffer, self->len);
}

template <typename T, size_t t_items_count>
void push_range(darr<T> *self, sbuff<T, t_items_count>&& items) {
    if (self->len + t_items_count > self->cap) {
        dresize( &self->buffer, &self->cap, max(t_items_count, 2 * (self->cap)) );
    }

    memcpy(self->buffer + self->len, items.buffer, t_items_count * sizeof(T));
    self->len += t_items_count;
}

template <typename T>
void append(darr<T> *self, darr<T> arr) {
    if (self->len + len(arr) > self->cap) {
        resize( &self->buffer, &self->cap, max(len(arr), 2 * self->cap) );
    }

    memcpy(self->buffer + self->len, arr.buffer, len(arr) * sizeof(T));
    self->len += len(arr);
}






}

