/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lselt.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/20 18:42:25 by abaur             #+#    #+#             */
/*   Updated: 2021/07/25 19:28:19 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lselt.h"

#include <stdlib.h>

extern void	lseltdelete(lselt* this){
	lselt* next;
	for (lselt* ilt=this; ilt!=NULL; ilt=next){
		next = ilt->next;
		if (ilt->child)
			lseltdelete(ilt->child);
		free(ilt);
	}
}

extern lselt**	lseltpush(lselt** this, const char* value){
	while(*this != NULL)
		this = &((**this).next);

	*this = malloc(sizeof(lselt));
	(**this).value = value;
	(**this).child = NULL;
	(**this).next  = NULL;

	return this;
}


#include <stdio.h>
static void	lseltloglvl(const lselt* this, int lvl){
	for (const lselt* ilt=this; ilt!=NULL; ilt=ilt->next){
		for (int i=lvl; i>0; i--)
			printf("\t");
		printf("-%s\n", ilt->value);
		if (ilt->child)
			lseltloglvl(ilt->child, lvl + 1);
	}
}

void	lseltlog(const lselt* this){
	lseltloglvl(this, 0);
}

