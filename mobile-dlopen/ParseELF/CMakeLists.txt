add_executable(ParseELF
    Main.cpp
    ParseElf.hpp
)

# install rules
install(
    TARGETS ParseELF
    DESTINATION .
)
