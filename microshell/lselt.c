/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lselt.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/20 18:42:25 by abaur             #+#    #+#             */
/*   Updated: 2021/07/20 19:04:10 by abaur            ###   ########.fr       */
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

extern void	lseltpush(lselt** this, const char* value){
	while(*this != NULL)
		this = &((**this).next);

	*this = malloc(sizeof(lselt));
	(**this).value = value;
	(**this).child = NULL;
	(**this).next  = NULL;
}
