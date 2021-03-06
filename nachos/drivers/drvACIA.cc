/* \file drvACIA.cc
   \brief Routines of the ACIA device driver
//
//      The ACIA is an asynchronous device (requests return
//      immediately, and an interrupt happens later on).
//      This is a layer on top of the ACIA.
//      Two working modes are to be implemented in assignment 2:
//      a Busy Waiting mode and an Interrupt mode. The Busy Waiting
//      mode implements a synchronous IO whereas IOs are asynchronous
//      IOs are implemented in the Interrupt mode (see the Nachos
//      roadmap for further details).
//
//  Copyright (c) 1999-2000 INSA de Rennes.
//  All rights reserved.
//  See copyright_insa.h for copyright notice and limitation
//  of liability and disclaimer of warranty provisions.
//
*/

/* Includes */

#include "kernel/system.h"         // for the ACIA object
#include "kernel/synch.h"
#include "machine/ACIA.h"
#include "drivers/drvACIA.h"

//-------------------------------------------------------------------------
// DriverACIA::DriverACIA()
/*! Constructor.
  Initialize the ACIA driver. In the ACIA Interrupt mode,
  initialize the reception index and semaphores and allow
  reception and emission interrupts.
  In the ACIA Busy Waiting mode, simply inittialize the ACIA
  working mode and create the semaphore.
  */
//-------------------------------------------------------------------------

DriverACIA::DriverACIA()
{
  #ifdef ETUDIANTS_TP
  send_sema = new Semaphore((char *) "Semaphore de send pour DriverACIA", 1);
  receive_sema = new Semaphore((char *) "Semaphore de receive pour DriverACIA", 0);
  ind_send = 0;
  ind_rec = 0;
  g_machine->acia->SetWorkingMode(REC_INTERRUPT);
  #endif

  #ifndef ETUDIANTS_TP
  printf("**** Warning: contructor of the ACIA driver not implemented yet\n");
  exit(-1);
  #endif
}

//-------------------------------------------------------------------------
// DriverACIA::TtySend(char* buff)
/*! Routine to send a message through the ACIA (Busy Waiting or Interrupt mode)
  */
//-------------------------------------------------------------------------

int DriverACIA::TtySend(char* buff) {
#ifdef ETUDIANTS_TP
    int number_send = 0;
    if(g_machine->acia->GetWorkingMode() == BUSY_WAITING) {
      DEBUG('d', (char *) "Début de l'envoie en attente active (driver ACIA)\n");
      while (buff[ind_send] != '\0') {
        if (g_machine->acia->GetOutputStateReg() == EMPTY) {
          char debug = buff[ind_send];
          DEBUG('d', (char *) "Envoie du char %c\n", debug);
          g_machine->acia->PutChar(buff[ind_send]);
          ind_send++;
          number_send++;
        }
      }
      //g_machine->acia->PutChar(buff[ind_send]); ne marche pas, pour les test on doit être sûr
      DEBUG('d', (char *) "Fin de l'envoie en mode BusyWaiting\n");
      ind_send = 0;
      DEBUG('d', (char *) "On return %d\n", number_send);
    }
    else {
        DEBUG('d', (char *) "Debut du send, ind_send = %d\n", ind_send);
        send_sema->P();
        ind_send = 0;
        strcpy(send_buffer, buff);
        number_send = sizeof buff + 1;
        g_machine->acia->SetWorkingMode(SEND_INTERRUPT);
        g_machine->acia->PutChar(send_buffer[ind_send]);
        }

  return number_send;
  #endif

  #ifndef ETUDIANTS_TP
  printf("**** Warning: contructor of the ACIA driver not implemented yet\n");
  exit(-1);
  return 0;
  #endif
}

//-------------------------------------------------------------------------
// DriverACIA::TtyReceive(char* buff,int length)
/*! Routine to reveive a message through the ACIA
//  (Busy Waiting and Interrupt mode).
  */
//-------------------------------------------------------------------------

int DriverACIA::TtyReceive(char* buff,int lg)
{
  #ifdef ETUDIANTS_TP
  int number_read = 0;

  if(g_machine->acia->GetWorkingMode() == BUSY_WAITING) {
    DEBUG('d', (char *) "Début de la réception en mode attente active (driver ACIA) \n");
    while (number_read < lg) {
      if (g_machine->acia->GetInputStateReg() == FULL) {
          char get = g_machine->acia->GetChar();
          DEBUG('d', (char *) "Le char %c à été reçu\n", get);
          /*if (get == '\0') {
            buff[number_read] = get;
            DEBUG('d', (char *) "Fin de la chaîne en avance, on s'arrète \n");
            return number_read++;
          } Ne marche pas, cf rapport*/
          buff[number_read] = get;
          number_read++;
      }
    }
  ind_rec = 0;
  DEBUG('d', (char *) "Fin de la réception, on a lu %d charactères\n", number_read);
  }

  else{
    receive_sema->P();
    memcpy(buff, receive_buffer, sizeof receive_buffer);
    g_machine->acia->SetWorkingMode(REC_INTERRUPT);
  }


  return number_read;
  #endif

  #ifndef ETUDIANTS_TP
  printf("**** Warning: method Tty_Receive of the ACIA driver not implemented yet\n");
  exit(-1);
  return 0;
  #endif
}


//-------------------------------------------------------------------------
// DriverACIA::InterruptSend()
/*! Emission interrupt handler.
  Used in the ACIA Interrupt mode only.
  Detects when it's the end of the message (if so, releases the send_sema semaphore), else sends the next character according to index ind_send.
  */
//-------------------------------------------------------------------------

void DriverACIA::InterruptSend()

#ifdef ETUDIANTS_TP
{
  DEBUG('d', (char *) "Debut de la routine d'interruption en emission \n");
  ind_send++;
  g_machine->acia->PutChar(send_buffer[ind_send]);
  DEBUG('d', (char *) "On a envoye le char : %c\n", send_buffer[ind_send]);
  if(send_buffer[ind_send] == '\0'){
    DEBUG('d', (char *) "On atteind la fin du message a envoyer\n");
    g_machine->acia->SetWorkingMode(REC_INTERRUPT);
    send_sema->V();
  }
  DEBUG('d', (char *) "A la fin de la routine on est en mode %d \n", g_machine->acia->GetWorkingMode());
}
#endif

#ifndef ETUDIANTS_TP
{
  printf("**** Warning: send interrupt handler not implemented yet\n");
  exit(-1);
}
#endif

//-------------------------------------------------------------------------
// DriverACIA::Interrupt_receive()
/*! Reception interrupt handler.
  Used in the ACIA Interrupt mode only. Reveices a character through the ACIA.
  Releases the receive_sema semaphore and disables reception
  interrupts when the last character of the message is received
  (character '\0').
  */
//-------------------------------------------------------------------------

void DriverACIA::InterruptReceive()
{
#ifdef ETUDIANTS_TP
    DEBUG('d', (char *) "Debut de la routine d'interruption en reception \n");
    char receive = g_machine->acia->GetChar();
    DEBUG('d', (char *) "On recoit le char : %c\n", receive);
    ind_rec++;
    receive_buffer[ind_rec] = receive;
    if(receive == '\0'){
        DEBUG('d', (char *) "On a recu le dernier char, on stop \n");
        g_machine->acia->SetWorkingMode(SEND_INTERRUPT);
        receive_sema->V();
    }
#endif

#ifndef ETUDIANTS_TP
  printf("**** Warning: receive interrupt handler not implemented yet\n");
  exit(-1);
#endif
}
