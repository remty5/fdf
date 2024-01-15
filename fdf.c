/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 16:57:19 by rvandepu          #+#    #+#             */
/*   Updated: 2024/01/12 09:07:13 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

#define WIDTH 720
#define HEIGHT 480

void	ft_hook(void *state)
{
	static int	x = 0;
	static int	y = 0;

	if (y == HEIGHT && x == WIDTH)
		return ;
	mlx_put_pixel(((t_fdf *)state)->screen, x, y, 0xFF00FFFF);
	x++;
	if (x == WIDTH)
	{
		x = 0;
		y++;
	}
}

// TODO finir fdf
int	main(void)
{
	t_fdf	fdf;

	fdf.mlx = mlx_init(WIDTH, HEIGHT, "fdf", false);
	if (!fdf.mlx)
		return (EXIT_FAILURE);
	fdf.screen = mlx_new_image(fdf.mlx, WIDTH, HEIGHT);
	if (!fdf.screen || (mlx_image_to_window(fdf.mlx, fdf.screen, 0, 0) == -1))
		return (EXIT_FAILURE);
	mlx_put_pixel(fdf.screen, 0, 0, 0xFF0000FF);
	draw_line(&fdf, (t_vec2){5, 5}, (t_vec2){500, 300});
	draw_line(&fdf, (t_vec2){400, 5}, (t_vec2){5, 400});
	mlx_loop_hook(fdf.mlx, &ft_hook, &fdf);
	mlx_loop(fdf.mlx);
	mlx_terminate(fdf.mlx);
	return (EXIT_SUCCESS);
}
