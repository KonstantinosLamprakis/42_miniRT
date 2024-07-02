/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_rays.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flfische <flfische@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 17:48:28 by flfische          #+#    #+#             */
/*   Updated: 2024/07/02 10:30:39 by flfische         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	ft_ray_init(t_ray *ray, t_vector3 *origin, t_vector3 *direction)
{
	ray->origin = origin;
	ray->direction = direction;
}

// static uint32_t	ft_get_void_color(void)
// {
// 	static t_bool	initialized = FALSE;
// 	t_color			color;
// 	static uint32_t	color_int;
// 	t_object		*ambient_light;

// 	if (!initialized)
// 	{
// 		ambient_light = ft_get_first_obj(AMBIENT_LIGHT);
// 		color = (t_color){0, 0, 0};
// 		ft_color_color_add(color, ambient_light->color_f, &color);
// 		ft_color_float_mult(color, ambient_light->s_ambient_light.ratio,
// 			&color);
// 		ft_color_float_mult(color, BACKGROUND_FACTOR, &color);
// 		color_int = ft_color_from_float(color);
// 		initialized = TRUE;
// 	}
// 	return (color_int);
// }

/**
 * Traces a ray and returns the color of the closest object hit.
 *
 * @param ray The ray to trace.
 * @return The color of the closest object hit.
 */
uint32_t	ft_trace_ray(t_ray *ray)
{
	int			i;
	double		closest;
	t_hit		hit;
	t_color		color;
	t_program	*program;

	i = 0;
	closest = INFINITY;
	program = ft_get_program();
	color = (t_color){0, 0, 0};
	hit.t = INFINITY;
	while (i < program->objs_len)
	{
		if (program->objs[i].type > LIGHT)
			ft_hit(ray, &program->objs[i], &hit);
		i++;
	}
	if (hit.t < closest)
		ft_compute_lights(&color, &hit, program);
	else
		return (program->void_color);
	return (ft_color_from_float(color));
}

/**
 * Sends a ray for a pixel.
 *
 * @param x The x coordinate of the pixel.
 * @param y The y coordinate of the pixel.
 * @param camera The camera.
 * @return The color of the pixel.
 */
uint32_t	ft_send_ray(int x, int y, t_object *camera)
{
	t_ray		ray;
	t_vector2	uv;
	t_vector3	temp_vec;
	double		randoff;

	ray.origin = &camera->pos;
	randoff = 0;
	if (ft_get_program()->max_samples > 1)
		randoff = ft_randf();
	uv.x = ((double)x + randoff) / (double)(WIN_WIDTH - 1);
	if (ft_get_program()->max_samples > 1)
		randoff = ft_randf();
	uv.y = 1 - ((double)y + randoff) / (double)(WIN_HEIGHT - 1);
	ray.direction = &(t_vector3){camera->s_camera.ll_corner->x,
		camera->s_camera.ll_corner->y, camera->s_camera.ll_corner->z};
	ft_v3_init(&temp_vec, camera->s_camera.hor->x * uv.x,
		camera->s_camera.hor->y * uv.x, camera->s_camera.hor->z * uv.x);
	ft_v3_add_ip(ray.direction, &temp_vec);
	ft_v3_init(&temp_vec, camera->s_camera.ver->x * uv.y,
		camera->s_camera.ver->y * uv.y, camera->s_camera.ver->z * uv.y);
	ft_v3_add_ip(ray.direction, &temp_vec);
	ft_v3_sub_ip(ray.direction, &camera->pos);
	ray.depth = ft_get_program()->bounces;
	return (ft_trace_ray(&ray));
}
