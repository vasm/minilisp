TEMPLATE = app
CONFIG -= qt

SOURCES += $$files($$PWD/minilisp/*.cpp)
HEADERS += $$files($$PWD/minilisp/*.hpp) \
    minilisp/eval.hpp
