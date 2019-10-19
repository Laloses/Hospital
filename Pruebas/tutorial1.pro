QT += widgets testlib

SOURCES = testqstring.cpp
QT       += core gui sql
# install
target.path = $$[QT_INSTALL_EXAMPLES]/qtestlib/tutorial1
INSTALLS += target

HEADERS +=
