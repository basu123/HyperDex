#!/usr/bin/env ruby
require 'hyperdex'

def assert &block
    raise RuntimeError unless yield
end

def collapse_iterator(xs)
    s = Set.new []
    while xs.has_next() do
        x = xs.next()
        x.freeze
        s.add(x)
    end
    s
end

def to_objectset(xs)
    s = Set.new []
    xs.each do |x|
        x.freeze
        s.add(x)
    end
    s
end

c = HyperDex::Client::Client.new(ARGV[0], ARGV[1].to_i)
assert { c.put("kv", "A", {"v" => "A"}) == true }
assert { c.put("kv", "B", {"v" => "B"}) == true }
assert { c.put("kv", "C", {"v" => "C"}) == true }
assert { c.put("kv", "D", {"v" => "D"}) == true }
assert { c.put("kv", "E", {"v" => "E"}) == true }
X0 = collapse_iterator(c.search("kv", {"k" => (HyperDex::Client::LessEqual.new "C")}))
Y0 = to_objectset([{:k => "A", :v => "A"}, {:k => "B", :v => "B"}, {:k => "C", :v => "C"}])
assert { X0.subset?Y0 and Y0.subset?X0 }
X1 = collapse_iterator(c.search("kv", {"v" => (HyperDex::Client::LessEqual.new "C")}))
Y1 = to_objectset([{:k => "A", :v => "A"}, {:k => "B", :v => "B"}, {:k => "C", :v => "C"}])
assert { X1.subset?Y1 and Y1.subset?X1 }
X2 = collapse_iterator(c.search("kv", {"k" => (HyperDex::Client::GreaterEqual.new "C")}))
Y2 = to_objectset([{:k => "C", :v => "C"}, {:k => "D", :v => "D"}, {:k => "E", :v => "E"}])
assert { X2.subset?Y2 and Y2.subset?X2 }
X3 = collapse_iterator(c.search("kv", {"v" => (HyperDex::Client::GreaterEqual.new "C")}))
Y3 = to_objectset([{:k => "C", :v => "C"}, {:k => "D", :v => "D"}, {:k => "E", :v => "E"}])
assert { X3.subset?Y3 and Y3.subset?X3 }
X4 = collapse_iterator(c.search("kv", {"k" => (HyperDex::Client::Range.new "B", "D")}))
Y4 = to_objectset([{:k => "B", :v => "B"}, {:k => "C", :v => "C"}, {:k => "D", :v => "D"}])
assert { X4.subset?Y4 and Y4.subset?X4 }
X5 = collapse_iterator(c.search("kv", {"v" => (HyperDex::Client::Range.new "B", "D")}))
Y5 = to_objectset([{:k => "B", :v => "B"}, {:k => "C", :v => "C"}, {:k => "D", :v => "D"}])
assert { X5.subset?Y5 and Y5.subset?X5 }
