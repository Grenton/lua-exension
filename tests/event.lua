print("Event object tests:")
ob1 = Base("ob1", "uuid_ob1", nil)
ob2 = Base("ob2", "uuid_ob2", nil)
ev1  = Event("ev1", "uuid_ev1", ob1)
ev2  = Event("ev2", "uuid_ev2", ob1)
ev3  = Event("ev2", "uuid_ev2", ob2)

ev1h1Started = false
ev1h1Stopped = false
ev1h1Src = nil
ev1:addCallback(function(src) 
    ev1h1Started = true
    ev1h1Src = src
    ev1h1Stopped = true
end)

ev1h2Started = false
ev1h2Stopped = false
ev1h2Src = nil
ev1:addCallback(function(src) 
    ev1h2Started = true
    msleep(1000)
    ev1h2Src = src
    ev1h2Stopped = true
end)

ev1h3Started = false
ev1h3Stopped = false
ev1h3Src = nil
ev1:addCallback(function(src) 
    ev1h3Started = true
    msleep(1000)
    ev1h3Src = src
    error("Random error here")
    ev1h3Stopped = true
end)


ev1h4Started = false
ev1h4Stopped = false
ev1h4Src = nil
ev1:addCallback(function(src) 
    ev1h4Started = true
    msleep(1000)
    ev1h4Src = src
    ev1h4Stopped = true
end)

ev2h1Started = false
ev2h1Stopped = false
ev2h1Src = nil
ev2:addCallback(function(src) 
    ev2h1Started = true
    msleep(3000)
    ev2h1Src = src
    ev2h1Stopped = true
end)

ev3h1Started = false
ev3h1Stopped = false
ev3h1Src = nil
ev3:addCallback(function(src) 
    ev3h1Started = true
    msleep(3000)
    ev3h1Src = src
    ev3h1Stopped = true
end)


ev1()
ev2()
msleep(500)

assert(ev1h1Started == true)
assert(ev1h1Stopped == true)
assert(ev1h1Src ~= nil)
assert(ev1h1Src.name == "ob1")
assert(ev1h2Started == true)
assert(ev1h2Stopped == false)
assert(ev1h2Src == nil)
assert(ev1h3Started == true)
assert(ev1h3Stopped == false)
assert(ev1h3Src == nil)
assert(ev1h4Started == true)
assert(ev1h4Stopped == false)
assert(ev1h4Src == nil)
assert(ev2h1Started == true)
assert(ev2h1Stopped == false)
assert(ev2h1Src == nil)
assert(ev3h1Started == false)
assert(ev3h1Stopped == false)
assert(ev3h1Src == nil)

msleep(500)
ev3()
msleep(500)

assert(ev1h1Started == true)
assert(ev1h1Stopped == true)
assert(ev1h1Src ~= nil)
assert(ev1h1Src.name == "ob1")
assert(ev1h2Started == true)
assert(ev1h2Stopped == true)
assert(ev1h2Src ~= nil)
assert(ev1h2Src.name == "ob1")
assert(ev1h3Started == true)
-- Event finished but ev1h3Stopped not set due to error in handler
assert(ev1h3Stopped == false)
assert(ev1h3Src ~= nil)
assert(ev1h3Src.name == "ob1")
assert(ev1h4Started == true)
assert(ev1h4Stopped == true)
assert(ev1h4Src ~= nil)
assert(ev1h4Src.name == "ob1")
assert(ev2h1Started == true)
assert(ev2h1Stopped == false)
assert(ev2h1Src == nil)
assert(ev3h1Started == true)
assert(ev3h1Stopped == false)
assert(ev3h1Src == nil)

msleep(2000)

assert(ev1h1Started == true)
assert(ev1h1Stopped == true)
assert(ev1h1Src ~= nil)
assert(ev1h1Src.name == "ob1")
assert(ev1h2Started == true)
assert(ev1h2Stopped == true)
assert(ev1h2Src ~= nil)
assert(ev1h2Src.name == "ob1")
assert(ev1h3Started == true)
assert(ev1h3Stopped == false)
assert(ev1h3Src ~= nil)
assert(ev1h3Src.name == "ob1")
assert(ev1h4Started == true)
assert(ev1h4Stopped == true)
assert(ev1h4Src ~= nil)
assert(ev1h4Src.name == "ob1")
assert(ev2h1Started == true)
assert(ev2h1Stopped == true)
assert(ev2h1Src ~= nil)
assert(ev2h1Src.name == "ob1")
assert(ev3h1Started == true)
assert(ev3h1Stopped == false)
assert(ev3h1Src == nil)

msleep(1000)

assert(ev1h1Started == true)
assert(ev1h1Stopped == true)
assert(ev1h1Src ~= nil)
assert(ev1h1Src.name == "ob1")
assert(ev1h2Started == true)
assert(ev1h2Stopped == true)
assert(ev1h2Src ~= nil)
assert(ev1h2Src.name == "ob1")
assert(ev1h3Started == true)
assert(ev1h3Stopped == false)
assert(ev1h3Src ~= nil)
assert(ev1h3Src.name == "ob1")
assert(ev1h4Started == true)
assert(ev1h4Stopped == true)
assert(ev1h4Src ~= nil)
assert(ev1h4Src.name == "ob1")
assert(ev2h1Started == true)
assert(ev2h1Stopped == true)
assert(ev2h1Src ~= nil)
assert(ev2h1Src.name == "ob1")
assert(ev3h1Started == true)
assert(ev3h1Stopped == true)
assert(ev3h1Src ~= nil)
assert(ev3h1Src.name == "ob2")

print("OK")