// Copyright (c) 2011, Cornell University
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright notice,
//       this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of HyperDex nor the names of its contributors may be
//       used to endorse or promote products derived from this software without
//       specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#ifndef hyperdex_buffer_h_
#define hyperdex_buffer_h_

// e
#include <e/buffer.h>

namespace hyperdex
{

inline e::packer&
operator << (e::packer& lhs, const std::vector<e::buffer>& rhs)
{
    uint16_t elem = rhs.size();
    lhs << elem;

    for (size_t i = 0; i < rhs.size(); ++i)
    {
        uint32_t size = rhs[i].size();
        lhs << size << rhs[i];
    }

    return lhs;
}

inline e::unpacker&
operator >> (e::unpacker& lhs, std::vector<e::buffer>& rhs)
{
    uint16_t cols;
    lhs >> cols;

    for (size_t i = 0; i < cols; ++i)
    {
        uint32_t size;
        e::buffer tmp;
        lhs >> size >> e::buffer::sized(size, &tmp);
        rhs.push_back(tmp);
    }

    return lhs;
}

} // namespace hyperdex

#endif // hyperdex_buffer_h_