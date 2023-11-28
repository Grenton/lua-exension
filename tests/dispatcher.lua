print("Dispatcher namespace tests:")
ob1 = Base("ob1", "uuid_ob1", nil)
ob2 = Base("ob2", "uuid_ob2", nil)
ev1  = Event("ev1", "uuid_ev1", ob1)
ev2  = Event("ev2", "uuid_ev2", ob1)
ev3  = Event("ev2", "uuid_ev2", ob2)

ev1h1Started = false
ev1h1Stopped = false
ev1h1Src = nil
dispatcher.register(ev1, function(src) 
    ev1h1Started = true
    ev1h1Src = src
    ev1h1Stopped = true
end)

ev1h2Started = false
ev1h2Stopped = false
ev1h2Src = nil
dispatcher.register(ev1, function(src) 
    ev1h2Started = true
    msleep(1000)
    ev1h2Src = src
    ev1h2Stopped = true
end)

assert(ev1h1Started == false)
assert(ev1h1Stopped == false)
assert(ev1h1Src == nil)
assert(ev1h2Started == false)
assert(ev1h2Stopped == false)
assert(ev1h2Src == nil)

dispatcher.trigger(ev1)
msleep(500)

assert(ev1h1Started == true)
assert(ev1h1Stopped == true)
assert(ev1h1Src ~= nil)
assert(ev1h1Src.name == "ob1")
assert(ev1h2Started == true)
assert(ev1h2Stopped == false)
assert(ev1h2Src == nil)

msleep(1000)
assert(ev1h1Started == true)
assert(ev1h1Stopped == true)
assert(ev1h1Src ~= nil)
assert(ev1h1Src.name == "ob1")
assert(ev1h2Started == true)
assert(ev1h2Stopped == true)
assert(ev1h2Src ~= nil)
assert(ev1h2Src.name == "ob1")

print("OK")