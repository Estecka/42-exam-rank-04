/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lselt.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/20 18:42:02 by abaur             #+#    #+#             */
/*   Updated: 2021/07/20 19:26:48 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LSELT_H
#define LSELT_H

typedef struct s_lselt	lselt;
struct s_lselt
{
	lselt*	next;
	lselt*	child;
	const char*	value;
};

/*
** Recursively frees a list and all its content.
** @paral lselt* this	The root of the list.
*/
void	lseltdelete(lselt* this);

/*
** Appends a value at the end of a list;
** @param lselt** this	A pointer to the variable referencing any element in the list.
** 	If list is empty, the variable should point to NULL. It will then be set to the root of the new list.
** @return A pointer to the newly created list element.
*/
lselt*	lseltpush(lselt** this, const char* value);


#endif
