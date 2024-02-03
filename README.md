# SFMT Random Extension for Defold

This extension wraps SIMD-oriented Fast Mersenne Twister (SFMT) random number generator v1.5.1.

Official documentation http://www.math.sci.hiroshima-u.ac.jp/m-mat/MT/SFMT/

# What is SFMT?

SFMT is a new variant of Mersenne Twister (MT) introduced by Mutsuo Saito and Makoto Matsumoto in 2006. The algorithm was reported at MCQMC 2006. The article published in the proceedings of MCQMC2006. (see Prof. Matsumoto's Papers on random number generation.) SFMT is a Linear Feedbacked Shift Register (LFSR) generator that generates a 128-bit pseudorandom integer at one step. SFMT is designed with recent parallelism of modern CPUs, such as multi-stage pipelining and SIMD (e.g. 128-bit integer) instructions. It supports 32-bit and 64-bit integers, as well as double precision floating point as output.

SFMT is much faster than MT, in most platforms. Not only the speed, but also the dimensions of equidistributions at v-bit precision are improved. In addition, recovery from 0-excess initial state is much faster. See Master's Thesis of Mutsuo Saito for detail.

The following implementation SFMT19937 can be compiled in three possible platforms:

Standard C without SIMD instructions
CPUs with Intel's SSE2 instructions + C compiler which supports these feature
CPUs with PowerPC's AltiVec instructions + C compiler which supports these feature
In each platform, SFMT has better performance than MT.

We put tables of equidistribution of SFMT. They are 32bit and 64bit.

# Setup

Paste this URL as a dependency in your `game.project` file.

```
https://github.com/Lerg/extension-sfmtrandom/archive/master.zip
```

# API reference

### sfmtrandom.init_gen_rand(`seed`: _number_) -> `sfmt`: _lightuserdata_

Initializes the internal state array with a UINT32 integer seed.

- `seed`: _number_ UINT32 seed number.
- return: `sfmt`: _lightuserdata_ SFMT internal state.

---
### sfmtrandom.init_by_array(`array`: _table_) -> `sfmt`: _lightuserdata_

Initializes the internal state array with a UINT32 integer seed.

- `array`: _table_ The array of a UINT32 integers used as a seed.
- return: `sfmt`: _lightuserdata_ SFMT internal state.

---
### sfmtrandom.get_idstring(`sfmt`: _lightuserdata_) -> `identification`: _string_

Returns the identification string. The string shows the word size, the Mersenne exponent and all the parameters of this generator.

- `sfmt`: _lightuserdata_ SFMT internal state.
- return: `identification`: _string_ Identification string.

---
### sfmtrandom.get_min_array_size32(`sfmt`: _lightuserdata_) -> `size`: _number_

Returns the minimum size of an array used for the `fill_array32()` function.

- `sfmt`: _lightuserdata_ SFMT internal state.
- return: `size`: _number_ Size.

---
### sfmtrandom.get_min_array_size64(`sfmt`: _lightuserdata_) -> `size`: _number_

Returns the minimum size of an array used for the `fill_array64()` function.

- `sfmt`: _lightuserdata_ SFMT internal state.
- return: `size`: _number_ Size.

---
### sfmtrandom.fill_array32(`sfmt`: _lightuserdata_, `array`: _buffer_)

Generates pseudorandom UINT32 integers in the specified array buffer. The number of pseudorandom integers is specified by the array size, which must be at least 624 and a multiple of four. Requires a freshly initialized SFMT state that hasn't been used for other generating functions.

- `sfmt`: _lightuserdata_ SFMT internal state.
- `array`: _buffer_ A buffer created with a stream named `"array"` of type `buffer.VALUE_TYPE_UINT32` where pseudorandom UINT32 integers are filled by this function.

---
### sfmtrandom.fill_array64(`sfmt`: _lightuserdata_, `array`: _buffer_)

Generates pseudorandom UINT64 integers in the specified array buffer. The number of pseudorandom integers is specified by the array size, which must be at least 312 and a multiple of two. Requires a freshly initialized SFMT state that hasn't been used for other generating functions.

- `sfmt`: _lightuserdata_ SFMT internal state.
- `array`: _buffer_ A buffer created with a stream named `"array"` of type `buffer.VALUE_TYPE_UINT64` where pseudorandom UINT64 integers are filled by this function.

---
### sfmtrandom.genrand_uint32(`sfmt`: _lightuserdata_) -> `value`: _number_

Generates a UINT32 pseudorandom number.

- `sfmt`: _lightuserdata_ SFMT internal state.
- return: `value`: _number_ UINT32 random value.

---
### sfmtrandom.genrand_uint64(`sfmt`: _lightuserdata_) -> `value`: _string_

Generates a UINT64 pseudorandom number. The function `genrand_uint64()` should not be called after `genrand_uint32()` unless an new initialization is performed.

- `sfmt`: _lightuserdata_ SFMT internal state.
- return: `value`: _string_ UINT64 random value represented as an 8 character long string - memory representation of a UINT64 number.

---
### sfmtrandom.genrand_real1(`sfmt`: _lightuserdata_) -> `value`: _number_

Generates a random number on [0,1]-real-interval.

- `sfmt`: _lightuserdata_ SFMT internal state.
- return: `value`: _number_ Random value.

---
### sfmtrandom.genrand_real2(`sfmt`: _lightuserdata_) -> `value`: _number_

Generates a random number on [0,1)-real-interval.

- `sfmt`: _lightuserdata_ SFMT internal state.
- return: `value`: _number_ Random value.

---
### sfmtrandom.genrand_real3(`sfmt`: _lightuserdata_) -> `value`: _number_

Generates a random number on (0,1)-real-interval.

- `sfmt`: _lightuserdata_ SFMT internal state.
- return: `value`: _number_ Random value.

---
### sfmtrandom.genrand_res53(`sfmt`: _lightuserdata_) -> `value`: _number_

Generates a random number on [0,1)-real-interval with 53-bit resolution using a 64-bit integer.

- `sfmt`: _lightuserdata_ SFMT internal state.
- return: `value`: _number_ Random value.

---
### sfmtrandom.genrand_res53_mix(`sfmt`: _lightuserdata_) -> `value`: _number_

Generates a random number on [0,1)-real-interval with 53-bit resolution using two 32bit integers.

- `sfmt`: _lightuserdata_ SFMT internal state.
- return: `value`: _number_ Random value.

---
### sfmtrandom.d4(`sfmt`: _lightuserdata_) -> `value`: _number_

Generates a random integer on [1,4] interval like from a dice.

- `sfmt`: _lightuserdata_ SFMT internal state.
- return: `value`: _number_ Random value.

---
### sfmtrandom.d6(`sfmt`: _lightuserdata_) -> `value`: _number_

Generates a random integer on [1,6] interval like from a dice.

- `sfmt`: _lightuserdata_ SFMT internal state.
- return: `value`: _number_ Random value.

---
### sfmtrandom.d8(`sfmt`: _lightuserdata_) -> `value`: _number_

Generates a random integer on [1,8] interval like from a dice.

- `sfmt`: _lightuserdata_ SFMT internal state.
- return: `value`: _number_ Random value.

---
### sfmtrandom.d10(`sfmt`: _lightuserdata_) -> `value`: _number_

Generates a random integer on [1,10] interval like from a dice.

- `sfmt`: _lightuserdata_ SFMT internal state.
- return: `value`: _number_ Random value.

---
### sfmtrandom.d12(`sfmt`: _lightuserdata_) -> `value`: _number_

Generates a random integer on [1,12] interval like from a dice.

- `sfmt`: _lightuserdata_ SFMT internal state.
- return: `value`: _number_ Random value.

---
### sfmtrandom.d20(`sfmt`: _lightuserdata_) -> `value`: _number_

Generates a random integer on [1,20] interval like from a dice.

- `sfmt`: _lightuserdata_ SFMT internal state.
- return: `value`: _number_ Random value.

---
### sfmtrandom.toss(`sfmt`: _lightuserdata_) -> `value`: _number_

Generates a random integer on [0,1] interval like from a toss of a coin.

- `sfmt`: _lightuserdata_ SFMT internal state.
- return: `value`: _number_ Random value.

---
### sfmtrandom.card(`sfmt`: _lightuserdata_) -> `value`: _number_

Generates a random integer on [1,52] interval like picking a random card from a standard deck.

- `sfmt`: _lightuserdata_ SFMT internal state.
- return: `value`: _number_ Random value.

---
### sfmtrandom.card2(`sfmt`: _lightuserdata_) -> `suit`: _number_, `card`: _number_

Generates two random integers on [1,4] and [1,13] intervals like picking a random card suit and a random card from a suit.

- `sfmt`: _lightuserdata_ SFMT internal state.
- return: `suit`: _number_ Random suit [1,4], `card`: _number_ Random card [1,13].

---
### sfmtrandom.shuffle(`sfmt`: _lightuserdata_, `array`: _table_)

Shuffles the provided array in place.

- `sfmt`: _lightuserdata_ SFMT internal state.
- `array`: _table_ The array of values.

---
### sfmtrandom.delete(`sfmt`: _lightuserdata_)

Deletes the random generator and frees the alocated memory for it.

- `sfmt`: _lightuserdata_ SFMT internal state.

### Example

```language-lua
local sfmt = sfmtrandom.init_gen_rand(socket.gettime())

print('uint32', sfmtrandom.genrand_uint32(sfmt))
print('real2', sfmtrandom.genrand_real2(sfmt))
print('d6', sfmtrandom.d6(sfmt))

sfmtrandom.delete(sfmt)
sfmt = nil
```
