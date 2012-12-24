#include "testssh.h"
#include <QString>

int test()
{
    ssh_session my_ssh_session;
    int rc;

    // Open session and set options
    my_ssh_session = ssh_new();
    if (my_ssh_session == NULL)
      exit(-1);
    ssh_options_set(my_ssh_session, SSH_OPTIONS_HOST, "localhost");

    // Connect to server
    rc = ssh_connect(my_ssh_session);
    if (rc != SSH_OK)
    {
      fprintf(stderr, "Error connecting to localhost: %s\n",
              ssh_get_error(my_ssh_session));
      ssh_free(my_ssh_session);
      exit(-1);
    }

    // Verify the server's identity
    // For the source code of verify_knowhost(), check previous example
//    if (verify_knownhost(my_ssh_session) < 0)
//    {
//      ssh_disconnect(my_ssh_session);
//      ssh_free(my_ssh_session);
//      exit(-1);
//    }

    // Authenticate ourselves
    QString password("whcrosedu");
    rc = ssh_userauth_password(my_ssh_session, NULL, password.toLatin1());
    if (rc != SSH_AUTH_SUCCESS)
    {
      fprintf(stderr, "Error authenticating with password: %s\n",
              ssh_get_error(my_ssh_session));
      ssh_disconnect(my_ssh_session);
      ssh_free(my_ssh_session);
      exit(-1);
    }
    qDebug()<<"evrika";

    show_remote_processes(my_ssh_session);

    ssh_disconnect(my_ssh_session);
    ssh_free(my_ssh_session);
}

int show_remote_processes(ssh_session session)
{
//  ssh_channel channel;
//  int rc;
//  char buffer[1024];
//  unsigned int nbytes;

//  channel = ssh_channel_new(session);
//  if (channel == NULL)
//    return SSH_ERROR;

//  rc = ssh_channel_open_session(channel);
//  if (rc != SSH_OK)
//  {
//    ssh_channel_free(channel);
//    return rc;
//  }

//  rc = ssh_channel_request_exec(channel, "ps aux");
//  if (rc != SSH_OK)
//  {
//    ssh_channel_close(channel);
//    ssh_channel_free(channel);
//    return rc;
//  }

//  nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0);
//  while (nbytes > 0)
//  {
//    if (write(1, buffer, nbytes) != nbytes)
//    {
//      ssh_channel_close(channel);
//      ssh_channel_free(channel);
//      return SSH_ERROR;
//    }
//    nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0);
//  }

//  if (nbytes < 0)
//  {
//    ssh_channel_close(channel);
//    ssh_channel_free(channel);
//    return SSH_ERROR;
//  }

//    qDebug()<<buffer;

//  ssh_channel_send_eof(channel);
//  ssh_channel_close(channel);
//  ssh_channel_free(channel);

  return SSH_OK;
}
