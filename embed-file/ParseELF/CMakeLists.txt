add_executable(ParseELF
    ParseElf.cpp
    ParseElf.hpp
)

# install rules
install(
    TARGETS ParseELF
    DESTINATION .
)
