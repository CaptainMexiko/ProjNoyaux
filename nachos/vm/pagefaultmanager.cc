/*! \file pagefaultmanager.cc
Routines for the page fault managerPage Fault Manager
*/
//
//  Copyright (c) 1999-2000 INSA de Rennes.
//  All rights reserved.  
//  See copyright_insa.h for copyright notice and limitation 
//  of liability and disclaimer of warranty provisions.
//

#include "kernel/thread.h"
#include "vm/swapManager.h"
#include "vm/physMem.h"
#include "vm/pagefaultmanager.h"

PageFaultManager::PageFaultManager() {
}

// PageFaultManager::~PageFaultManager()
/*! Nothing for now
*/
PageFaultManager::~PageFaultManager() {
}

// ExceptionType PageFault(uint32_t virtualPage)
/*! 	
//	This method is called by the Memory Management Unit when there is a 
//      page fault. This method loads the page from :
//      - read-only sections (text,rodata) $\Rightarrow$ executive
//        file
//      - read/write sections (data,...) $\Rightarrow$ executive
//        file (1st time only), or swap file
//      - anonymous mappings (stack/bss) $\Rightarrow$ new
//        page from the MemoryManager (1st time only), or swap file
//
//	\param virtualPage the virtual page subject to the page fault
//	  (supposed to be between 0 and the
//        size of the address space, and supposed to correspond to a
//        page mapped to something [code/data/bss/...])
//	\return the exception (generally the NO_EXCEPTION constant)
*/  
ExceptionType PageFaultManager::PageFault(uint32_t virtualPage) 
{
#ifdef ETUDIANTS_TP

 if(g_machine->mmu->translationTable->getBitSwap(virtualPage)) {
      DEBUG('v', (char *) "Defaut de page, page dans la zone swap, debut du changement de page\n");
      //TODO verifier qu"il n'est pas de voleur de page
      char buff_tmp[g_cfg->PageSize];
      g_swap_manager->GetPageSwap(g_machine->mmu->translationTable->getAddrDisk(virtualPage), buff_tmp);
      int pp = g_physical_mem_manager->AddPhysicalToVirtualMapping(g_current_thread->GetProcessOwner()->addrspace, virtualPage);
      memcpy(&(g_machine->mainMemory[pp * g_cfg->PageSize]), &buff_tmp,
             sizeof(buff_tmp));
      g_machine->mmu->translationTable->setPhysicalPage(virtualPage,pp);
      g_machine->mmu->translationTable->setBitValid(virtualPage);
  }
  else if(!g_machine->mmu->translationTable->getBitSwap(virtualPage) && g_machine->mmu->translationTable->getAddrDisk(virtualPage)==-1){
      DEBUG('v', (char *) "Page anonyme, allouage de la page et mise a 0\n");
      int pp = g_physical_mem_manager->AddPhysicalToVirtualMapping(g_current_thread->GetProcessOwner()->addrspace, virtualPage);
      DEBUG('v', (char *) "On met la page a zero\n");
      char tmp_buff_anon[g_cfg->PageSize];
      memset(&tmp_buff_anon, 0, static_cast<size_t>(g_cfg->PageSize));
      printf("Affichage buffer tmp : %X\n", tmp_buff_anon);
      memcpy(&(g_machine->mainMemory[pp * g_cfg->PageSize]), &tmp_buff_anon,
              g_cfg->PageSize);
      g_machine->mmu->translationTable->setPhysicalPage(virtualPage,pp);
      DEBUG('v', (char *)"Mise a zero termine, on previent la mmu que la page est swapable\n");
      g_machine->mmu->translationTable->setAddrDisk(virtualPage, -1);
      DEBUG('v', (char *)"La page est valide\n");
      g_machine->mmu->translationTable->setBitValid(virtualPage);
      DEBUG('v', (char *)"Fin du defaut de page\n");
  } else {
      DEBUG('v', (char *) "Defaut de page, page a charger dans l'executable\n");
      char tmp_buff[g_cfg->PageSize];
      DEBUG('v', (char *) "On lit le fichier de code que l'on place dans un buffer\n");
      g_current_thread->GetProcessOwner()->exec_file->ReadAt(tmp_buff, g_cfg->PageSize, g_machine->mmu->translationTable->getAddrDisk(virtualPage));
      DEBUG('v', (char *) "On recupere une page physique\n");
      int pp = g_physical_mem_manager->AddPhysicalToVirtualMapping(g_current_thread->GetProcessOwner()->addrspace, virtualPage);
      DEBUG('v', (char *) "On copie sur la page reel le code recupere\n");
      memcpy(&g_machine->mainMemory[pp * g_cfg->PageSize], tmp_buff, g_cfg->PageSize);
      DEBUG('v', (char *) "Copie termine, on link la page reel et virtuel \n");
      g_machine->mmu->translationTable->setPhysicalPage(virtualPage,pp);
      g_machine->mmu->translationTable->setBitValid(virtualPage);
  }
return ((ExceptionType)0);

#endif

#ifndef ETUDIANTS_TP
  printf("**** Warning: page fault manager is not implemented yet\n");
    exit(-1);
    return ((ExceptionType)0);
#endif
}




