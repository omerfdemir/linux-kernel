#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/sched.h>
#include<linux/syscalls.h>
#include<linux/mm.h>
#include<asm/pgtable.h>
#include "va_to_pa.h"


asmlinkage long sys_VA_to_PA(unsigned long long va){
	pgd_t *pgd;
	pud_t *pud;
	pmd_t *pmd;
	pte_t *ptep,pte;
	unsigned long long physical_address;
	
	struct page *page = NULL;
	struct mm_struct *mm = current->mm;

	
	pgd = pgd_offset(mm, va);
	if (pgd_none(*pgd) || pgd_bad(*pgd))
		goto out;
	printk(KERN_NOTICE "Valid pgd");

	pud = pud_offset(pgd, va);
	if (pud_none(*pud) || pud_bad(*pud))
		goto out;
	printk(KERN_NOTICE "Valid pud");

	pmd = pmd_offset(pud, va);
	if (pmd_none(*pmd) || pmd_bad(*pmd))
		goto out;
	printk(KERN_NOTICE "Valid pmd");

	ptep = pte_offset_map(pmd, va);
	if (!ptep)
		goto out;
	pte = *ptep;

	page = pte_page(pte);
	if (page)
	physical_address = page_to_phys(page);
	printk("va 0x%llx -> pa 0x%llx\n",
	va, page_to_phys(page));

	pte_unmap(ptep);
out:
	return page;
return physical_address;

}







