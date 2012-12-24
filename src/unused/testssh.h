#ifndef TESTSSH_H
#define TESTSSH_H

#include <libssh/libssh.h>
#include <libssh/callbacks.h>
#include <libssh/server.h>
#include <libssh/ssh2.h>
#include <libssh/sftp.h>

#include <stdlib.h>
#include <QDebug>
#include <string.h>

int test();
int show_remote_processes(ssh_session session);

#endif // TESTSSH_H
