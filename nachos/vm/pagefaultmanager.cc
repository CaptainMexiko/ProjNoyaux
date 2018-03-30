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

  //chargement page manquante en zone swap
  if(g_machine->mmu->translationTable->getBitSwap(virtualPage)){
    char * buff_tmp;
    g_swap_manager->GetPageSwap(g_machine->mmu->translationTable->getAddrDisk(virtualPage), buff_tmp);
    int pp = g_physical_mem_manager->AddPhysicalToVirtualMapping(g_current_thread->GetProcessOwner()->addrspace, 1);

	  //memcpy(&g_physical_mem_manager->tpr[pp], &buff_tmp, sizeof buff_tmp)

	  g_machine->mmu->translationTable->setBitValid(virtualPage);
  }
  else if(!g_machine->mmu->translationTable->getBitSwap(virtualPage) && g_machine->mmu->translationTable->getAddrDisk(virtualPage)==-1){

      int pp = g_physical_mem_manager->AddPhysicalToVirtualMapping(g_current_thread->GetProcessOwner()->addrspace, 1);
      memset(&(g_machine->mainMemory[g_machine->mmu->translationTable->getPhysicalPage(virtualPage)*g_cfg->PageSize]), 0, g_cfg->PageSize);
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




