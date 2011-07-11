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

// C
#include <cassert>

// HyperDex
#include "hyperdex/hyperspace.h"
#include "hyperdex/search.h"

hyperdex :: search :: search(size_t n)
    : m_value(n)
    , m_mask(n)
{
}

hyperdex :: search :: ~search()
{
}

void
hyperdex :: search :: set(size_t idx, const e::buffer& val)
{
    assert(m_value.size() > idx);
    assert(m_mask.bits() > idx);

    m_mask.set(idx);
    m_value[idx] = val;
}

void
hyperdex :: search :: unset(size_t idx)
{
    assert(m_value.size() > idx);
    assert(m_mask.bits() > idx);

    m_mask.unset(idx);
    m_value[idx] = e::buffer();
}

void
hyperdex :: search :: clear()
{
    m_value = std::vector<e::buffer>(m_value.size());
    m_mask = e::bitfield(m_value.size());
}

bool
hyperdex :: search :: matches(const std::vector<e::buffer>& value)
                      const
{
    assert(m_value.size() == value.size());
    assert(m_mask.bits() == value.size());

    for (size_t i = 0; i < m_mask.bits(); ++i)
    {
        if (m_mask.get(i) && m_value[i] != value[i])
        {
            return false;
        }
    }

    return true;
}

uint32_t
hyperdex :: search :: secondary_point() const
{
    std::vector<uint64_t> hashes;

    for (size_t i = 0; i < m_value.size(); ++i)
    {
        if (m_mask.get(i))
        {
            hashes.push_back(CityHash64(m_value[i]));
        }
        else
        {
            hashes.push_back(0);
        }
    }

    return hyperspace::secondary_point(hashes);
}

uint32_t
hyperdex :: search :: secondary_mask() const
{
    std::vector<uint64_t> hashes;

    for (size_t i = 0; i < m_value.size(); ++i)
    {
        if (m_mask.get(i))
        {
            hashes.push_back(-1);
        }
        else
        {
            hashes.push_back(0);
        }
    }

    return hyperspace::secondary_point(hashes);
}

uint64_t
hyperdex :: search :: replication_point(const std::vector<bool>& dims) const
{
    std::vector<uint64_t> value_hashes;

    for (size_t i = 0; i < m_value.size(); ++i)
    {
        if (m_mask.get(i))
        {
            value_hashes.push_back(CityHash64(m_value[i]));
        }
        else
        {
            value_hashes.push_back(0);
        }
    }

    return hyperspace::replication_point(0, value_hashes, dims);
}

uint64_t
hyperdex :: search :: replication_mask(const std::vector<bool>& dims) const
{
    std::vector<uint64_t> value_hashes;

    for (size_t i = 0; i < m_value.size(); ++i)
    {
        if (m_mask.get(i))
        {
            value_hashes.push_back(-1);
        }
        else
        {
            value_hashes.push_back(0);
        }
    }

    return hyperspace::replication_point(0, value_hashes, dims);
}
