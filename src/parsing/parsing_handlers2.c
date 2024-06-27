/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_handlers2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flfische <flfische@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 15:43:05 by klamprak          #+#    #+#             */
/*   Updated: 2024/06/27 16:39:01 by flfische         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/**
 * @brief Get the cy object
 *
 * @param tokens
 * @param obj
 * @return int 1 on success, 0 otherwise
 */
int	get_cy(char **tokens, t_object *obj)
{
	obj->type = CYLINDER;
	if (get_arr_len(tokens) != 6)
		return (0);
	if (!get_vector(&obj->pos, tokens[1]))
		return (0);
	if (!get_vector(&obj->s_cylinder.normal, tokens[2]))
		return (0);
	if (!in_range(&obj->s_cylinder.normal, -1, 1))
		return (0);
	if (!is_float(tokens[3]) && !is_int(tokens[3]))
		return (0);
	obj->s_cylinder.diameter = (double)ft_atod(tokens[3]);
	if (!is_float(tokens[4]) && !is_int(tokens[4]))
		return (0);
	obj->s_cylinder.height = (double)ft_atod(tokens[4]);
	if (!get_color(tokens[5], &obj->color))
		return (0);
	ft_color_to_float(obj->color, &obj->color_f);
	ft_default_material(obj);
	ft_assign_own_tm(obj);
	return (1);
}

int	get_co(char **tokens, t_object *obj)
{
	obj->type = CONE;
	if (get_arr_len(tokens) != 6)
		return (0);
	if (!get_vector(&obj->pos, tokens[1]))
		return (0);
	if (!get_vector(&obj->s_cone.normal, tokens[2]))
		return (0);
	if (!in_range(&obj->s_cone.normal, -1, 1))
		return (0);
	if (!is_float(tokens[3]) && !is_int(tokens[3]))
		return (0);
	obj->s_cone.radius = (double)ft_atod(tokens[3]);
	if (!is_float(tokens[4]) && !is_int(tokens[4]))
		return (0);
	obj->s_cone.height = (double)ft_atod(tokens[4]);
	if (!get_color(tokens[5], &obj->color))
		return (0);
	ft_color_to_float(obj->color, &obj->color_f);
	obj->s_cone.angle = atan(obj->s_cone.radius / obj->s_cone.height) * 2;
	obj->s_cone.min = 0;
	ft_default_material(obj);
	ft_assign_own_tm(obj);
	return (1);
}
