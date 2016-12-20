/*****************************************************************************
 *   GATB : Genome Assembly Tool Box
 *   Copyright (C) 2014  INRIA
 *   Authors: R.Chikhi, G.Rizk, E.Drezen
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#define INT128_FOUND            1

#define STR_LIBRARY_VERSION     "1.2.1"
#define STR_COMPILATION_DATE    "2016-11-15/09:37:47"
#define STR_COMPILATION_FLAGS   " -O3 -DNDEBUG -Wall -Wno-unused-function -Wno-unused-variable -Wno-parentheses -Wno-format -Wno-strict-aliasing -Wno-array-bounds -Wno-unknown-pragmas  -DINT128_FOUND  -DWITH_LAMBDA_EXPRESSION -std=c++0x -DUSE_NEW_CXX     -DWITH_MPHF     -DEMPHF_USE_POPCOUNT=1 -Wno-invalid-offsetof"
#define STR_COMPILER            "/usr/bin/cc  (4.9.2)"

#define STR_OPERATING_SYSTEM    "Linux-4.4.27.1.amd64-smp"

#define KSIZE_LIST    32,64,96,128
#define KSIZE_STRING "32 64 96 128"

#define KSIZE_LIST_TYPE  boost::mpl::int_<32>,boost::mpl::int_<64>,boost::mpl::int_<96>,boost::mpl::int_<128>

#ifdef GATB_USE_CUSTOM_ALLOCATOR
    #define CUSTOM_MEM_ALLOC  1
#else
    #define CUSTOM_MEM_ALLOC  0
#endif

#define GATB_HDF5_NB_ITEMS_PER_BLOCK (4*1024)
#define GATB_HDF5_CLEANUP_WORKAROUND 4
