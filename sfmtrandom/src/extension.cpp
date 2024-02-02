#include "extension.h"
#include "SFMT/SFMT.h"

// Check if the first argument to a Lua function is the SFMT lightuserdata variable.
static sfmt_t *get_sfmt_lightuserda(lua_State *L) {
	if (lua_islightuserdata(L, 1)) {
		return (sfmt_t *)lua_touserdata(L, 1);
	} else {
		luaL_error(L, "Expected lightuserdata, got %s. Wrong type for the sfmt variable: '%s'.", luaL_typename(L, 1), lua_tostring(L, 1));
	}
	return nullptr;
}

// Check if the argument to a Lua function is a number variable.
static uint32_t get_uint32(lua_State *L, int index, const char *variable_name, bool *success) {
	if (lua_isnumber(L, index)) {
		*success = true;
		return lua_tointeger(L, index);
	} else {
		*success = false;
		luaL_error(L, "Expected number, got %s. Wrong type for the '%s' variable: '%s'.", luaL_typename(L, index), variable_name, lua_tostring(L, index));
	}
	return 0;
}

// Check if the argument to a Lua function is a table variable.
static bool is_table(lua_State *L, int index, const char *variable_name) {
	if (!lua_istable(L, index)) {
		luaL_error(L, "Expected table, got %s. Wrong type for the '%s' variable: '%s'.", luaL_typename(L, index), variable_name, lua_tostring(L, index));
		return false;
	}
	return true;
}

// Check if the argument to a Lua function is a buffer variable.
static bool is_buffer(lua_State *L, int index, const char *variable_name) {
	if (!dmScript::IsBuffer(L, index)) {
		luaL_error(L, "Expected buffer, got %s. Wrong type for the '%s' variable: '%s'.", luaL_typename(L, index), variable_name, lua_tostring(L, index));
		return false;
	}
	return true;
}

static int lua_init_gen_rand(lua_State *L) {
	bool success;
	uint32_t seed = get_uint32(L, 1, "seed", &success);
	if (success) {
		sfmt_t *sfmt = new sfmt_t;
		sfmt_init_gen_rand(sfmt, seed);
		lua_pushlightuserdata(L, sfmt);
		return 1;
	}
	return 0;
}

static int lua_init_by_array(lua_State *L) {
	int array_index = 1;
	if (is_table(L, array_index, "array")) {
		int key_length = lua_objlen(L, array_index);
		if (key_length > 0) {
			uint32_t init_key[key_length];
			bool success;
			for (int i = 1; i <= key_length; ++i) {
				lua_rawgeti(L, array_index, i);
				uint32_t value = get_uint32(L, -1, "array element", &success);
				if (success) {
					init_key[i - 1] = value;
				} else {
					break;
				}
			}
			if (success) {
				sfmt_t *sfmt = new sfmt_t;
				sfmt_init_by_array(sfmt, init_key, key_length);
				lua_pushlightuserdata(L, sfmt);
				return 1;
			}
		} else {
			luaL_error(L, "The array must not be empty");
		}
	}
	return 0;
}

static int lua_get_idstring(lua_State *L) {
	sfmt_t *sfmt = get_sfmt_lightuserda(L);
	if (sfmt != nullptr) {
		lua_pushstring(L, sfmt_get_idstring(sfmt));
		return 1;
	}
	return 0;
}

static int lua_get_min_array_size32(lua_State *L) {
	sfmt_t *sfmt = get_sfmt_lightuserda(L);
	if (sfmt != nullptr) {
		lua_pushinteger(L, sfmt_get_min_array_size32(sfmt));
		return 1;
	}
	return 0;
}

static int lua_get_min_array_size64(lua_State *L) {
	sfmt_t *sfmt = get_sfmt_lightuserda(L);
	if (sfmt != nullptr) {
		lua_pushinteger(L, sfmt_get_min_array_size64(sfmt));
		return 1;
	}
	return 0;
}

static int lua_fill_array32(lua_State *L) {
	sfmt_t *sfmt = get_sfmt_lightuserda(L);
	if (sfmt != nullptr) {
		int array_index = 2;
		if (is_buffer(L, array_index, "array")) {
			dmBuffer::HBuffer buffer = dmScript::CheckBufferUnpack(L, array_index);
			dmhash_t stream_name = dmHashString64("array");
			dmBuffer::ValueType stream_type;
			uint32_t components = 0;
			dmBuffer::Result r = dmBuffer::GetStreamType(buffer, stream_name, &stream_type, &components);
			if (r == dmBuffer::RESULT_OK) {
				if (stream_type == dmBuffer::VALUE_TYPE_UINT32) {
					uint32_t *array = nullptr;
					uint32_t size = 0;
					uint32_t stride = 0;
					r = dmBuffer::GetStream(buffer, stream_name, (void**)&array, &size, &components, &stride);
					if (r == dmBuffer::RESULT_OK) {
						if (size % 4 != 0) {
							luaL_error(L, "Wrong size of the 'array' stream of the array buffer, must be a multiple of 4");
						} else {
							sfmt_fill_array32(sfmt, array, size);
						}
					} else {
						luaL_error(L, "Can't get the 'array' stream of the array buffer");
					}
				} else {
					luaL_error(L, "Wrong type of the 'array' stream of the array buffer, expected VALUE_TYPE_UINT32");
				}
			} else {
				luaL_error(L, "Can't get the 'array' stream type of the array buffer");
			}
		}
	}
	return 0;
}

static int lua_fill_array64(lua_State *L) {
	sfmt_t *sfmt = get_sfmt_lightuserda(L);
	if (sfmt != nullptr) {
		int array_index = 2;
		if (is_buffer(L, array_index, "array")) {
			dmBuffer::HBuffer buffer = dmScript::CheckBufferUnpack(L, array_index);
			dmhash_t stream_name = dmHashString64("array");
			dmBuffer::ValueType stream_type;
			uint32_t components = 0;
			dmBuffer::Result r = dmBuffer::GetStreamType(buffer, stream_name, &stream_type, &components);
			if (r == dmBuffer::RESULT_OK) {
				if (stream_type == dmBuffer::VALUE_TYPE_UINT64) {
					uint64_t *array = nullptr;
					uint32_t size = 0;
					uint32_t stride = 0;
					r = dmBuffer::GetStream(buffer, stream_name, (void**)&array, &size, &components, &stride);
					if (r == dmBuffer::RESULT_OK) {
						if (size % 2 != 0) {
							luaL_error(L, "Wrong size of the 'array' stream of the array buffer, must be a multiple of 2");
						} else {
							sfmt_fill_array64(sfmt, array, size);
						}
					} else {
						luaL_error(L, "Can't get the 'array' stream of the array buffer");
					}
				} else {
					luaL_error(L, "Wrong type of the 'array' stream of the array buffer, expected VALUE_TYPE_UINT64");
				}
			} else {
				luaL_error(L, "Can't get the 'array' stream type of the array buffer");
			}
		}
	}
	return 0;
}

static int lua_genrand_uint32(lua_State *L) {
	sfmt_t *sfmt = get_sfmt_lightuserda(L);
	if (sfmt != nullptr) {
		lua_pushinteger(L, sfmt_genrand_uint32(sfmt));
		return 1;
	}
	return 0;
}

static int lua_genrand_uint64(lua_State *L) {
	sfmt_t *sfmt = get_sfmt_lightuserda(L);
	if (sfmt != nullptr) {
		uint64_t value = sfmt_genrand_uint64(sfmt);
		uint8_t buffer[8];
		int i;
		for (i = 0; i < 8; ++i) {
			buffer[i] = (value >> (i * 8)) & 0xff;
		}
		lua_pushlstring(L, (const char *)buffer, 8);
		return 1;
	}
	return 0;
}

static int lua_genrand_real1(lua_State *L) {
	sfmt_t *sfmt = get_sfmt_lightuserda(L);
	if (sfmt != nullptr) {
		lua_pushnumber(L, sfmt_genrand_real1(sfmt));
		return 1;
	}
	return 0;
}

static int lua_genrand_real2(lua_State *L) {
	sfmt_t *sfmt = get_sfmt_lightuserda(L);
	if (sfmt != nullptr) {
		lua_pushnumber(L, sfmt_genrand_real2(sfmt));
		return 1;
	}
	return 0;
}

static int lua_genrand_real3(lua_State *L) {
	sfmt_t *sfmt = get_sfmt_lightuserda(L);
	if (sfmt != nullptr) {
		lua_pushnumber(L, sfmt_genrand_real3(sfmt));
		return 1;
	}
	return 0;
}

static int lua_genrand_res53(lua_State *L) {
	sfmt_t *sfmt = get_sfmt_lightuserda(L);
	if (sfmt != nullptr) {
		lua_pushnumber(L, sfmt_genrand_res53(sfmt));
		return 1;
	}
	return 0;
}

static int lua_genrand_res53_mix(lua_State *L) {
	sfmt_t *sfmt = get_sfmt_lightuserda(L);
	if (sfmt != nullptr) {
		lua_pushnumber(L, sfmt_genrand_res53_mix(sfmt));
		return 1;
	}
	return 0;
}

static int lua_d4(lua_State *L) {
	sfmt_t *sfmt = get_sfmt_lightuserda(L);
	if (sfmt != nullptr) {
		lua_pushnumber(L, floor(4.0 * sfmt_genrand_real2(sfmt) + 1.0));
		return 1;
	}
	return 0;
}

static int lua_d6(lua_State *L) {
	sfmt_t *sfmt = get_sfmt_lightuserda(L);
	if (sfmt != nullptr) {
		lua_pushnumber(L, floor(6.0 * sfmt_genrand_real2(sfmt) + 1.0));
		return 1;
	}
	return 0;
}

static int lua_d8(lua_State *L) {
	sfmt_t *sfmt = get_sfmt_lightuserda(L);
	if (sfmt != nullptr) {
		lua_pushnumber(L, floor(8.0 * sfmt_genrand_real2(sfmt) + 1.0));
		return 1;
	}
	return 0;
}

static int lua_d10(lua_State *L) {
	sfmt_t *sfmt = get_sfmt_lightuserda(L);
	if (sfmt != nullptr) {
		lua_pushnumber(L, floor(10.0 * sfmt_genrand_real2(sfmt) + 1.0));
		return 1;
	}
	return 0;
}

static int lua_d12(lua_State *L) {
	sfmt_t *sfmt = get_sfmt_lightuserda(L);
	if (sfmt != nullptr) {
		lua_pushnumber(L, floor(12.0 * sfmt_genrand_real2(sfmt) + 1.0));
		return 1;
	}
	return 0;
}

static int lua_d20(lua_State *L) {
	sfmt_t *sfmt = get_sfmt_lightuserda(L);
	if (sfmt != nullptr) {
		lua_pushnumber(L, floor(20.0 * sfmt_genrand_real2(sfmt) + 1.0));
		return 1;
	}
	return 0;
}

static int lua_toss(lua_State *L) {
	sfmt_t *sfmt = get_sfmt_lightuserda(L);
	if (sfmt != nullptr) {
		lua_pushnumber(L, floor(2.0 * sfmt_genrand_real2(sfmt)));
		return 1;
	}
	return 0;
}

static int lua_card(lua_State *L) {
	sfmt_t *sfmt = get_sfmt_lightuserda(L);
	if (sfmt != nullptr) {
		lua_pushnumber(L, floor(52.0 * sfmt_genrand_real2(sfmt) + 1.0));
		return 2;
	}
	return 0;
}

static int lua_card2(lua_State *L) {
	sfmt_t *sfmt = get_sfmt_lightuserda(L);
	if (sfmt != nullptr) {
		lua_pushnumber(L, floor(4.0 * sfmt_genrand_real2(sfmt) + 1.0));
		lua_pushnumber(L, floor(13.0 * sfmt_genrand_real2(sfmt) + 1.0));
		return 2;
	}
	return 0;
}

// Delete the random generator and free the alocated memory for it.
static int lua_delete(lua_State *L) {
	sfmt_t *sfmt = get_sfmt_lightuserda(L);
	if (sfmt != nullptr) {
		delete sfmt;
	}
	return 0;
}

static const luaL_reg lua_functions[] = {
	{"init_gen_rand", lua_init_gen_rand},
	{"init_by_array", lua_init_by_array},
	{"get_idstring", lua_get_idstring},
	{"get_min_array_size32", lua_get_min_array_size32},
	{"get_min_array_size64", lua_get_min_array_size64},
	{"fill_array32", lua_fill_array32},
	{"fill_array64", lua_fill_array64},
	{"genrand_uint32", lua_genrand_uint32},
	{"genrand_uint64", lua_genrand_uint64},
	{"genrand_real1", lua_genrand_real1},
	{"genrand_real2", lua_genrand_real2},
	{"genrand_real3", lua_genrand_real3},
	{"genrand_res53", lua_genrand_res53},
	{"genrand_res53_mix", lua_genrand_res53_mix},
	{"d4", lua_d4},
	{"d6", lua_d6},
	{"d8", lua_d8},
	{"d10", lua_d10},
	{"d12", lua_d12},
	{"d20", lua_d20},
	{"toss", lua_toss},
	{"card", lua_card},
	{"card2", lua_card2},
	{"delete", lua_delete},
	{0, 0}
};

dmExtension::Result APP_INITIALIZE(dmExtension::AppParams *params) {
	return dmExtension::RESULT_OK;
}

dmExtension::Result APP_FINALIZE(dmExtension::AppParams *params) {
	return dmExtension::RESULT_OK;
}

dmExtension::Result INITIALIZE(dmExtension::Params *params) {
	luaL_register(params->m_L, EXTENSION_NAME_STRING, lua_functions);
	lua_pop(params->m_L, 1);
	return dmExtension::RESULT_OK;
}

dmExtension::Result UPDATE(dmExtension::Params *params) {
	return dmExtension::RESULT_OK;
}

void EXTENSION_ON_EVENT(dmExtension::Params *params, const dmExtension::Event *event) {}

dmExtension::Result FINALIZE(dmExtension::Params *params) {
	return dmExtension::RESULT_OK;
}

DM_DECLARE_EXTENSION(EXTENSION_NAME, EXTENSION_NAME_STRING, APP_INITIALIZE, APP_FINALIZE, INITIALIZE, UPDATE, EXTENSION_ON_EVENT, FINALIZE)
