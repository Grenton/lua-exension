print("Base Object tests:")
parentObject = Base("parentName", "4321", nil)
parentObject2 = Base("parentName", "4321", nil)
object = Base("name", "1234", parentObject)
assert(object.uuid == "1234")
assert(object.name == "name")
assert(object:getName() == "name")
assert(type(object) == "Base")

object:addTag("Tag1")
object:addTag("Tag2")
object:addTag("Tag3")
object:addTag("Tag4")
object:addTag("Tag5")
object:removeTag("Tag2")
object:removeTag("Tag4")

tags = object:getTags()

assert(tags[1] == "Tag1")
assert(tags[2] == "Tag3")
assert(tags[3] == "Tag5")

parent = object:getParent()
assert(parent == parentObject)
assert(parent ~= parentObject2)

print("OK")
