#!/usr/bin/env python
import sys
import hyperdex.client
from hyperdex.client import LessEqual, GreaterEqual, Range, Regex, LengthEquals, LengthLessEqual, LengthGreaterEqual
c = hyperdex.client.Client(sys.argv[1], int(sys.argv[2]))
def to_objectset(xs):
    return set([frozenset(x.items()) for x in xs])
assert c.put('kv', 'k', {}) == True
assert c.get('kv', 'k') == {'v': {}}
assert c.put('kv', 'k', {'v': {1: 7, 2: 8, 3: 9}}) == True
assert c.get('kv', 'k') == {'v': {1: 7, 2: 8, 3: 9}}
assert c.put('kv', 'k', {'v': {}}) == True
assert c.get('kv', 'k') == {'v': {}}
