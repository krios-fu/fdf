/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krios-fu <krios-fu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/11 19:03:59 by krios-fu          #+#    #+#             */
/*   Updated: 2021/06/14 21:52:34 by krios-fu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/fdf.h"

void	my_mlx_pixel_put(t_fdf *fdf, int x, int y, int color)
{
	char *dst;

	dst = (char *)fdf->data->addr + (y * fdf->data->line_lenght + x
		* (fdf->data->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

void	print_menu(t_fdf **fdf)
{
	char *menu;

	menu = "Move picture: w, s, d, a";
	mlx_string_put((*fdf)->data->mlx, (*fdf)->data->win, 42, 42, 0x03fc35, menu);
	menu = "Zoom: Up, Dowm ";
	mlx_string_put((*fdf)->data->mlx, (*fdf)->data->win, 42, 62, 0x03fc35, menu);
	menu = "Rotation: Rigth, Left";
	mlx_string_put((*fdf)->data->mlx, (*fdf)->data->win, 42, 82, 0x03fc35, menu);
	menu = "2D, 3D: p, i";
	mlx_string_put((*fdf)->data->mlx, (*fdf)->data->win, 42, 102, 0x03fc35, menu);
	menu = "Move Angle: q, e";
	mlx_string_put((*fdf)->data->mlx, (*fdf)->data->win, 42, 122, 0x03fc35, menu);
	menu = "Mood Universe: SP";
	mlx_string_put((*fdf)->data->mlx, (*fdf)->data->win, 42, 122, 0x03fc35, menu);
}

void print_star(t_fdf **fdf)
{
	int i;

	i = 0;
	if((*fdf)->cam->keycode->sp)
	{
		while (i < 100)
		{
			mlx_string_put((*fdf)->data->mlx, (*fdf)->data->win, rand () % 1080 , rand () %  720  , 0xffc700, ".");
			mlx_string_put((*fdf)->data->mlx, (*fdf)->data->win, rand () % 1080   , rand () %  720 , 0xffffff, ".");
			i++;
		}		
	}
}

int		ft_exit(t_fdf **fdf)
{
	write(1, "\x1b[36mClose\n", 11);
	mlx_destroy_window((*fdf)->data->mlx, (*fdf)->data->win);
	exit(0);
}

void fill_check(t_fdf **fdf, char *str)
{
	check_map(str, fdf);
	fiil_map(str, fdf);
	get_color(str, fdf);
}

int start(t_fdf **fdf)
{	
	key_move(fdf);
	 mlx_destroy_image((*fdf)->data->mlx, (*fdf)->data->img);
	 (*fdf)->data->img = mlx_new_image((*fdf)->data->mlx, (*fdf)->data->winx, (*fdf)->data->winy);
	 draw(fdf);
	 mlx_put_image_to_window((*fdf)->data->mlx,
	 (*fdf)->data->win, (*fdf)->data->img, 0, 0);
	 print_star(fdf);
	 print_menu(fdf);
	return(1);
}

int main(int argc, char *argv[])
{
	t_fdf *fdf;
	t_map map;
	t_data data;
	t_cam  cam;
	t_keycode keycode;
	
	fdf = (t_fdf *)malloc(sizeof(t_fdf));
	fdf->map = &map;
	fdf->map->zoom = 3;
	fdf->data = &data;
	fdf->data->arg = argc;
	fdf->data->winx = 1080;
	fdf->data->winy = 720;
	fdf->cam = &cam;
	fdf->cam->keycode = &keycode;


	fdf->cam->keycode->w = 0;
	fdf->cam->keycode->a = 0;
	fdf->cam->keycode->s = 0;
	fdf->cam->keycode->d = 0;
	fdf->cam->keycode->zu = 0;
	fdf->cam->keycode->zd = 0;
	fdf->cam->keycode->rr = 0;
	fdf->cam->keycode->rl = 0;
	fdf->cam->keycode->p = 0;
	fdf->cam->keycode->i = 0;
	fdf->cam->keycode->sp = 0;
	fdf->cam->plane = 0;
	fdf->cam->rotspeed = 0.8;
	fdf->cam->keycode->e = 0;
	fdf->cam->keycode->q = 0;
	fdf->cam->angle = 0.8;
	fdf->cam->keycode->c = 0;
	fdf->map->eleva = 1;
	fdf->cam->keycode->x = 0;
	fdf->cam->keycode->esc = 0;
	
	
	
	fill_check(&fdf, argv[1]);

	fdf->cam->dir_x = (int)fdf->data->winx / 2;
	fdf->cam->dir_y = (int)(fdf->data->winy / 3);
	fdf->data->mlx = mlx_init();
	fdf->data->win =
		mlx_new_window(fdf->data->mlx, fdf->data->winx, fdf->data->winy, "FdF");
	fdf->data->img = mlx_new_image(fdf->data->mlx, fdf->data->winx, fdf->data->winy);
	fdf->data->addr = mlx_get_data_addr(fdf->data->img, &fdf->data->bits_per_pixel,
	 &fdf->data->line_lenght, &fdf->data->endian);

	draw(&fdf);
	mlx_put_image_to_window(fdf->data->mlx,
		fdf->data->win, fdf->data->img, 0, 0);
	print_menu(&fdf);
	print_star(&fdf);
	//draw(&fdf);
	mlx_hook(fdf->data->win, 2, (1L << 0), ft_keypress,&fdf);
	mlx_hook(fdf->data->win, 3, (1L << 0), ft_keyrelease, &fdf);
	mlx_hook(fdf->data->win, 17, (17L << 0), ft_exit, &fdf);
	//mlx_loop_hook(fdf->data->win, start, &fdf);
	mlx_loop(fdf->data->mlx);
	return(0);
}