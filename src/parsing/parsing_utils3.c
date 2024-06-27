/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klamprak <klamprak@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 22:46:57 by klamprak          #+#    #+#             */
/*   Updated: 2024/06/27 12:58:42 by klamprak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/**
 * @brief free an NULL terminated array of char pointers properly
 * ex. arr = ["El1", "el2", NULL]
 *
 * @param arr
 */
void	free_str_arr(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = -1;
	while (arr[++i])
		free(arr[i]);
	free(arr);
}

/**
 * @brief Get the nbr of lines, count how many lines there are in a file,
 * skipping empty lines
 *
 * @param fname the file which will read and count how many lines it contains
 * @return int -1 if its an error opening-closing file, nbr of lines otherwise
 */
int	get_nbr_of_lines(char *fname)
{
	char	*line;
	int		count;
	int		fd;
	char	**tokens;

	fd = open(fname, O_RDONLY);
	if (fd == -1)
		return (print_err_extend("Err opening file: ", strerror(errno), 0), -1);
	line = get_next_line(fd);
	count = 0;
	while (line)
	{
		tokens = ft_multi_split(line, " \t");
		if (!((line[0] == '\n' && !line[1]) || !tokens || !tokens[0]
				|| tokens[0][0] == '#'))
			count++;
		free(line);
		if (tokens)
			free_str_arr(tokens);
		line = get_next_line(fd);
	}
	if (close(fd) == -1)
		return (print_err_extend("Err closing file: ", strerror(errno), 0), -1);
	return (count);
}

/**
 * @brief combine three integers [0-255] to one uint32_t number
 *
 * @param red
 * @param green
 * @param blue
 * @return uint32_t
 */
uint32_t	int_to_rgb(int red, int green, int blue)
{
	uint32_t	color;

	color = 0;
	color |= (uint32_t)red << 24;
	color |= (uint32_t)green << 16;
	color |= (uint32_t)blue << 8;
	color |= 0xFF;
	return (color);
}

/**
 * @brief checks if exists exactly one A, C and L
 * (Ambient light, Camera and Light)
 *
 * @param t_object **objs null terminated objs list which will be checked
 * @return int returns 1 if is valid number, 0 otherwise
 */
int	is_valid_obj_nbr(t_object *objs)
{
	int	i;
	int	single_occur[OBJECT_COUNT];

	i = 0;
	while (i < OBJECT_COUNT)
		single_occur[i++] = 0;
	if (!objs)
		return (0);
	i = -1;
	while (++i < ft_get_program()->objs_len)
		single_occur[objs[i].type]++;
	if (single_occur[AMBIENT_LIGHT] != 1)
		return (ft_print_error("Expect exact one A"), free(objs), 0);
	if (single_occur[CAMERA] != 1)
		return (ft_print_error("Expect exact one C"), free(objs), 0);
	ft_get_program()->objs = objs;
	return (1);
}
