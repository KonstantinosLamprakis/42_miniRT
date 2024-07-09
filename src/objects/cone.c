/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klamprak <klamprak@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 15:39:34 by klamprak          #+#    #+#             */
/*   Updated: 2024/07/09 13:17:13 by klamprak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static void	calc_abc(t_ray *ray, t_object *cone, double *abc)
{
	t_vector3	delta_p;
	double	tmp_v[3];

	ft_v3_init(&delta_p, ray->origin->x - cone->pos.x,
	ray->origin->y - cone->pos.y, ray->origin->z - cone->pos.z);
	tmp_v[0] = 1 + ((cone->s_cone.radius / cone->s_cone.height) *
	(cone->s_cone.radius / cone->s_cone.height));
	tmp_v[1] = ft_v3_dotprod(ray->direction, &cone->s_cone.normal);
	tmp_v[2] = ft_v3_dotprod(&delta_p, &cone->s_cone.normal);
	abc[0] = ft_v3_dotprod(ray->direction, ray->direction) -
	tmp_v[0] * powf(tmp_v[1], 2);
	abc[1] = 2 * (ft_v3_dotprod(ray->direction, &delta_p) -
	tmp_v[0] * tmp_v[1] * tmp_v[2]);
	abc[2] = ft_v3_dotprod(&delta_p, &delta_p) - tmp_v[0] * powf(tmp_v[2], 2);
}

static bool	calc_hit(t_ray *ray, t_object *cone, double dist, double *t)
{
	t_vector3	p;
	double	distance;

	ft_v3_init(&p, ray->origin->x + ray->direction->x * dist,
		ray->origin->y + ray->direction->y * dist, ray->origin->z
		+ ray->direction->z * dist);
	ft_v3_sub_ip(&p, &cone->pos);
	distance = ft_v3_dotprod(&p, &cone->s_cone.normal);
	if (distance >= 0 && distance <= cone->s_cone.height)
		if (dist < *t)
			*t = dist;
	if (distance >= 0 && distance <= cone->s_cone.height)
		return (true);
	return (false);
}

static bool	check_cone_body(t_ray *ray, t_object *cone, double *abc, double *t)
{
	bool	hit;
	double	tmp;
	double	t0;
	double	t1;

	tmp = INFINITY;
	if (!is_sol_equation(abc, &t0, &t1))
		return (false);
	hit = calc_hit(ray, cone, t0, &tmp);
	hit = calc_hit(ray, cone, t1, &tmp) || hit;
	if (hit)
		*t = tmp;
	return (hit);
}

void	get_cone_plane(t_object *cone, t_object *to_init_pl)
{
	t_vector3	center;
	t_vector3	normal;
	t_vector3	middle_offset;

	ft_v3_init(&normal, cone->s_cone.normal.x,
		cone->s_cone.normal.y, cone->s_cone.normal.z);
	ft_v3_init(&center, cone->pos.x, cone->pos.y, cone->pos.z);
	ft_v3_init(&middle_offset, normal.x * cone->s_cone.height,
		normal.y * cone->s_cone.height, normal.z
		* cone->s_cone.height);
	ft_v3_scalar_ip(&center, 1 + EPSILON);
	ft_v3_add_ip(&center, &middle_offset);
	ft_v3_init(&to_init_pl->pos, center.x, center.y, center.z);
	ft_v3_init(&to_init_pl->s_plane.normal, normal.x, normal.y, normal.z);
	ft_v3_normal_ip(&to_init_pl->s_plane.normal);
}

double	ft_cone_hit(t_object *cone, t_ray *ray)
{
	t_object	tmp_cone;
	double		abc[3];
	t_object	bot_pl;
	bool		hit;
	double		t;

	t = INFINITY;
	ft_v3_init(&tmp_cone.pos, cone->pos.x, cone->pos.y, cone->pos.z);
	ft_v3_init(&tmp_cone.s_cone.normal, cone->s_cone.normal.x, cone->s_cone.normal.y, cone->s_cone.normal.z);
	tmp_cone.s_cone.height = cone->s_cone.height;
	tmp_cone.s_cone.radius = cone->s_cone.radius;
	ft_v3_init(&tmp_cone.s_cone.normal, cone->s_cone.normal.x, cone->s_cone.normal.y, cone->s_cone.normal.z);
	ft_v3_div_ip(&tmp_cone.s_cone.normal, sqrt(ft_v3_dotprod(&tmp_cone.s_cone.normal, &tmp_cone.s_cone.normal)));
	get_cone_plane(&tmp_cone, &bot_pl);
	calc_abc(ray, &tmp_cone, abc);
	hit = check_cone_body(ray, &tmp_cone, abc, &t);
	hit = inter_disk(ray, &bot_pl, &t, cone->s_cone.radius) || hit;
	if (hit)
		return (t);
	return (INFINITY);
}

int	ft_cone_normal(t_hit *hit, t_ray *ray)
{
	t_vector3	tmp1;
	t_vector3	tmp2;
	double		radius;

	ft_v3_init(&hit->p, ray->origin->x + ray->direction->x * hit->t,
		ray->origin->y + ray->direction->y * hit->t, ray->origin->z
		+ ray->direction->z * hit->t);
	ft_v3_init(&tmp2, hit->p.x - hit->obj->pos.x, hit->p.y - hit->obj->pos.y, hit->p.z - hit->obj->pos.z);
	ft_v3_init(&tmp1, hit->obj->s_cone.normal.x, hit->obj->s_cone.normal.y, hit->obj->s_cone.normal.z);
	ft_v3_scalar_ip(&tmp1, ft_v3_dotprod(&tmp2, &hit->obj->s_cone.normal));
	ft_v3_sub_ip(&tmp2, &tmp1);
	radius = hit->obj->s_cone.radius;
	ft_v3_scalar_ip(&tmp2, 1.0 / (radius / hit->obj->s_cone.height));
	ft_v3_add_ip(&tmp1, &tmp2);
	ft_v3_init(&hit->n, tmp1.x, tmp1.y, tmp1.z);
	return (1);
}
