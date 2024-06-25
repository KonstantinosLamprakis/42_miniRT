/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klamprak <klamprak@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 15:39:34 by klamprak          #+#    #+#             */
/*   Updated: 2024/06/25 15:57:10 by klamprak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static float	in_height(float t[2], t_object *cone, float CO_A, float D_A);

float	ft_cone_hit(t_object *cone, t_ray *ray)
{
	t_vector3	co;
	float		cos_theta2;
	float		dot_dc[2];
	float		abc[4];
	float		t[2];

	ft_v3_init(&co, ray->origin->x - cone->pos.x, ray->origin->y - cone->pos.y \
	, ray->origin->z - cone->pos.z);
	cos_theta2 = cos(cone->s_cone.angle) * cos(cone->s_cone.angle);
	dot_dc[0] = ft_v3_dotprod(ray->direction, &cone->s_cone.normal);
	dot_dc[1] = ft_v3_dotprod(&co, &cone->s_cone.normal);
	abc[0] = ft_v3_dotprod(ray->direction, ray->direction) - (cos_theta2 + 1)
		* (dot_dc[0] * dot_dc[0]);
	abc[1] = 2 * (ft_v3_dotprod(ray->direction, &co) - \
	(cos_theta2 + 1) * dot_dc[0]
			* dot_dc[1]);
	abc[3] = ft_v3_dotprod(&co, &co) - (cos_theta2 + 1) * \
	(dot_dc[1] * dot_dc[1]);
	abc[3] = abc[1] * abc[1] - 4 * abc[0] * abc[3];
	if (abc[3] < 0 || abc[0] == 0)
		return (INFINITY);
	t[0] = (-abc[1] - sqrt(abc[3])) / (2 * abc[0]);
	t[1] = (-abc[1] + sqrt(abc[3])) / (2 * abc[0]);
	return (in_height(t, cone, dot_dc[1], dot_dc[0]));
}

static float	in_height(float t[2], t_object *cone, float CO_A, float D_A)
{
	float	hit_height[2];

	if (t[0] > 0)
	{
		hit_height[0] = CO_A + t[0] * D_A;
		if ((hit_height[0] >= cone->s_cone.min && \
		hit_height[0] <= cone->s_cone.max))
			return (t[0]);
	}
	if (t[1] > 0)
	{
		hit_height[1] = CO_A + t[1] * D_A;
		if ((hit_height[1] >= cone->s_cone.min && \
		hit_height[1] <= cone->s_cone.max))
			return (t[1]);
	}
	return (INFINITY);
}

int	ft_cone_normal(t_hit *hit, t_ray *ray)
{
	t_vector3	tmp;
	float		projec;
	float		magnitude;
	float		angle;

	ft_v3_init(&hit->p, ray->origin->x + ray->direction->x * hit->t,
		ray->origin->y + ray->direction->y * hit->t, ray->origin->z
		+ ray->direction->z * hit->t);
	ft_v3_init(&hit->n, hit->p.x, hit->p.y, hit->p.z);
	ft_v3_sub_ip(&hit->n, &hit->obj->pos);
	projec = ft_v3_dotprod(&hit->n, &hit->obj->s_cone.normal);
	magnitude = sqrt(ft_v3_dotprod(&hit->n, &hit->n) - (projec * projec));
	if (projec == 0)
		return (0);
	angle = atan(magnitude / projec);
	ft_v3_init(&tmp, hit->obj->s_cone.normal.x, hit->obj->s_cone.normal.y, \
	hit->obj->s_cone.normal.z);
	ft_v3_scalar_ip(&tmp, projec * (1 + (tan(angle) * tan(angle))));
	ft_v3_sub_ip(&hit->n, &tmp);
	ft_v3_normal_ip(&hit->n);
	return (1);
}
