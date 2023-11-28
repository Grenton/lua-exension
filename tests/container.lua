print("Container Object tests:")
object1 = Base("name1", "1111", nil)
object2 = Base("name2", "2222", nil)
object3 = Base("name3", "3333", nil)

object1:addTag("Tag1")
object2:addTag("Tag1")
object3:addTag("Tag1")

object1:addTag("Tag2")
object3:addTag("Tag2")

object2:addTag("Tag3")

container = Container()
container:addObject(object1)
container:addObject(object2)
container:addObject(object3)

objects = container:getObjects()
assert(objects[1] == object1)
assert(objects[2] == object2)
assert(objects[3] == object3)

objects = container:getByTag("Tag1")
assert(objects[1] == object1)
assert(objects[2] == object2)
assert(objects[3] == object3)

objects = container:getByTag("Tag2")
assert(objects[1] == object1)
assert(objects[2] == object3)

objects = container:getByTag("Tag3")
assert(objects[1] == object2)

object1:removeTag("Tag1")
objects = container:getByTag("Tag1")
assert(objects[1] == object2)
assert(objects[2] == object3)

container:removeObject(object2)
objects = container:getObjects()
assert(objects[1] == object1)
assert(objects[2] == object3)

print("OK")
