/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klamprak <klamprak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 16:32:02 by klamprak          #+#    #+#             */
/*   Updated: 2024/06/09 18:53:48 by klamprak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/**
 * @brief checks if str is a sequence of digits and nothing else
 *
 * @param str
 * @return int 1 if it contains only digits, 0 otherwise
 */
int	is_int(char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	i = 0;
	while (str[i])
		if (!ft_isdigit(str[i++]))
			return (0);
	return (1);
}

/**
 * @brief checks if str contains only digits and a '.' in between
 *
 * @param str
 * @return int 1 if it match the case, 0 otherwise
 */
int	is_float(char *str)
{
	char	**parts;

	if (ft_strnstr(str, "..", ft_strlen(str)))
		return (0);
	parts = ft_split(str, '.');
	if (get_arr_len(parts) != 2)
		return (free_str_arr(parts), 0);
	if (!is_int(parts[0]) || !is_int(parts[1]))
		return (free_str_arr(parts), 0);
	return (free_str_arr(parts), 1);
}

/**
 * @brief get x, z and y from a string
 * ex. "13.4,12,0.1" -> x=13.4, y=12, z=0.1
 *
 * @param vector
 * @param str
 * @return int 1 if succeed, 0 if not int or not floats or not 3 numbers
 */
int	get_vector(t_vector3 *vector, char *str)
{
	char	**nums;
	int		i;
	float	result[3];

	nums = ft_split(str, ',');
	if (get_arr_len(nums) != 3)
		return (free_str_arr(nums), 0);
	i = 0;
	while (nums[i])
	{
		if (is_int(nums[i]))
			result[i] = ft_atoi(nums[i]);
		else if (is_float(nums[i]))
			result[i] = ft_atod(nums[i]);
		else
			return (free_str_arr(nums), 0);
		i++;
	}
	vector->x = result[0];
	vector->y = result[1];
	vector->z = result[2];
	return (1);
}

/**
 * @brief check if vector's values are on range [start, end] included
 *
 * @param vector
 * @param start
 * @param end
 * @return int 1 if its on range, 0 otherwise
 */
int	in_range(t_vector3 *vector, float start, float end)
{
	if (vector->x < start || vector->x > end)
		return (0);
	if (vector->y < start || vector->y > end)
		return (0);
	if (vector->z < start || vector->z > end)
		return (0);
	return (1);
}
