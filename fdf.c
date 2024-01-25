/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 16:57:19 by rvandepu          #+#    #+#             */
/*   Updated: 2024/01/22 17:23:24 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

// TODO assert node exists before calling
void	traverse(t_fdf *fdf, t_btree *node, t_point *last)
{
	t_point	next;

	if (last == NULL)
		return (next = (t_point){{fdf->scale, fdf->scale},
			convert_to_col(node->color)}, traverse(fdf, node, &next));
	//ft_printf("[node] val: %d\n", node->value);
	//ft_printf("       color: 0x%X\n", node->color);
	node->checked = !node->checked;
	if (node->left)
	{
		//ft_printf("[%2d]   left: %d\n", node->value, node->left->value);
		next = (t_point){{last->pos.x, last->pos.y + fdf->scale}, convert_to_col(node->left->color)};
		draw_line(fdf, *last, &next);
		if (node->checked != node->left->checked)
		{
			//ft_printf("[%2d]   going left...\n", node->value);
			traverse(fdf, node->left, &next);
		}
	}
	if (node->right)
	{
		//ft_printf("[%2d]   right: %d\n", node->value, node->right->value);
		next = (t_point){{last->pos.x + fdf->scale, last->pos.y}, convert_to_col(node->right->color)};
		draw_line(fdf, *last, &next);
		if (node->checked != node->right->checked)
		{
			//ft_printf("[%2d]   going right...\n", node->value);
			traverse(fdf, node->right, &next);
		}
	}
}

static int	parse_args(t_fdf *fdf, int argc, char *argv[])
{
	if (argc < 2)
		return (ft_printf("Usage: %s <file> [--fullscreen] [color] [alpha]\n",
				argv[0]), -1);
	fdf->args.map_path = argv[1];
	if (argc > 2)
		fdf->args.fullscreen = !ft_strcmp(argv[2], "--fullscreen");
	if (argc > 3)
		fdf->args.default_color = parse_nbr_skip(&argv[3]);
	else
		fdf->args.default_color = DEFAULT_COLOR;
	if (argc > 4)
		fdf->args.default_alpha = parse_nbr_skip(&argv[4]);
	else
		fdf->args.default_alpha = DEFAULT_ALPHA;
	if (fdf->args.default_color < 0 || fdf->args.default_alpha < 0)
		return (-1);
	return (0);
}

static void	init_fdf(t_fdf *fdf)
{
	fdf->width = 480;
	fdf->height = 320;
	if (fdf->args.fullscreen)
	{
		mlx_set_setting(MLX_HEADLESS, true);
		fdf->mlx = mlx_init(fdf->width, fdf->height, "", false);
		mlx_get_monitor_size(0, &fdf->width, &fdf->height);
		mlx_terminate(fdf->mlx);
		mlx_set_setting(MLX_HEADLESS, false);
	}
	mlx_set_setting(MLX_FULLSCREEN, fdf->args.fullscreen);
	fdf->mlx = mlx_init(fdf->width, fdf->height, "fdf", !fdf->args.fullscreen);
	fdf->zoom = 1;
	fdf->scale = 2;
	fdf->res = 0.5;
}

static void	ft_hook(void *state)
{
	t_fdf		*fdf;
	static int	i = 0;

	fdf = state;
	ft_printf("rendering i:%d\n", i);
	traverse(fdf, fdf->mesh->values, NULL);
	i++;
}

int	main(int argc, char *argv[])
{
	t_fdf	fdf;

	if (parse_args(&fdf, argc, argv) < 0)
		return (EXIT_FAILURE);
	ft_printf("loading map: %s\n", argv[1]);
	load_map(&fdf);
	ft_printf("mesh loaded: %p\n", fdf.mesh);
	if (fdf.mesh == NULL)
		return (EXIT_FAILURE);
	ft_printf("starting init\n", fdf.width, fdf.height);
	init_fdf(&fdf);
	ft_printf("init done, width: %d, height: %d\n", fdf.width, fdf.height);
	if (!fdf.mlx)
		return (EXIT_FAILURE);
	fdf.screen = mlx_new_image(fdf.mlx, fdf.width, fdf.height);
	if (!fdf.screen || (mlx_image_to_window(fdf.mlx, fdf.screen, 0, 0) == -1))
		return (EXIT_FAILURE);
	ft_printf("starting render...\n");
	if (fdf.mesh->values)
		traverse(&fdf, fdf.mesh->values, NULL);
	ft_printf("render complete\n");
	mlx_loop_hook(fdf.mlx, &ft_hook, &fdf);
	mlx_loop(fdf.mlx);
	mlx_terminate(fdf.mlx);
	free_mesh(fdf.mesh, NULL);
	return (EXIT_SUCCESS);
}
