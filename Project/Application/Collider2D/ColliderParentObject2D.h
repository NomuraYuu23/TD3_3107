#pragma once
#include <variant>

class Sample;
class Player;
class SampleObject;
class IObject;

using ColliderParentObject2D = std::variant<Sample*, Player*, SampleObject*, IObject*>;
