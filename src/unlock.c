/*
   Unlocking the doors at Bitraf Hackerspace Oslo

   Written by:
   Vidar Kristiansen <glinnovators@gmail.com>
   Alexander Alemayhu <alexander@bitraf.no>
   Martin Stensgård <mastensg@ping.uio.no>
 */

#include <err.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#include <wiringPi.h>

enum pin
{
  OUTERDOOR = 12,
  INNERDOOR = 14,
  LED = 11
};

static void
signal_handler(int s)
{
  digitalWrite (OUTERDOOR, 0);
  digitalWrite (INNERDOOR, 0);
  digitalWrite (LED, 0);

  errx (EXIT_FAILURE, "Got signal: %d", s);
}

int
main (void)
{
  signal (SIGHUP, signal_handler);
  signal (SIGINT, signal_handler);
  signal (SIGQUIT, signal_handler);
  signal (SIGILL, signal_handler);
  signal (SIGABRT, signal_handler);
  signal (SIGFPE, signal_handler);
  signal (SIGKILL, signal_handler);
  signal (SIGSEGV, signal_handler);
  signal (SIGPIPE, signal_handler);
  signal (SIGALRM, signal_handler);
  signal (SIGTERM, signal_handler);
  signal (SIGUSR1, signal_handler);
  signal (SIGUSR2, signal_handler);
  signal (SIGCHLD, signal_handler);
  signal (SIGCONT, signal_handler);
  signal (SIGSTOP, signal_handler);
  signal (SIGTSTP, signal_handler);
  signal (SIGTTIN, signal_handler);
  signal (SIGTTOU, signal_handler);

  alarm(90);

  if (-1 == wiringPiSetup ())
    errx (EXIT_FAILURE, "wiringPiSetup");

  pinMode (LED, OUTPUT);
  pinMode (OUTERDOOR, OUTPUT);
  pinMode (INNERDOOR, OUTPUT);

  digitalWrite (LED, 1);

  digitalWrite (OUTERDOOR, 1);
  digitalWrite (INNERDOOR, 1);

  sleep (5);
  digitalWrite (OUTERDOOR, 0);

  sleep (90);
  digitalWrite (INNERDOOR, 0);

  digitalWrite (LED, 0);

  return EXIT_SUCCESS;
}
