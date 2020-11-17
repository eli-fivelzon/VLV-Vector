# VLV-Vector
An implementation for a generic, dynamically growing vector (with a similar API to std::vector).
The vector memory loaction changes between the stack and the heap as needed (with static memory size as
a template parameter).
Was developed as part of a school assignment.

# Usage
include VLV-Vector.hpp in your source file, then
create an instance of VLV-Vector<T, StaticCapacity>
where T is the type of the values stored in the vector
and static capacity is the amount of stack memory you
want the vector to have.
