# FdF

[Subject](https://cdn.intra.42.fr/pdf/pdf/185030/en.subject.pdf)

This project is about creating a simple wireframe model representation of a 3D landscape by linking various points (x, y, z) thanks to line segments (edges).

## Instructions

1. Clone repo
```bash
git clone https://github.com/m0d1nst4ll3r/43_FdF.git && cd 43_FdF
```
2. Clone mlx
```bash
git clone https://github.com/42paris/minilibx-linux.git
```
3. Clone libft
```bash
git clone https://github.com/m0d1nst4ll3r/43_mylibft.git
```
4. Run make
```bash
make
```
5. Run fdf
```bash
./fdf
```

If make fails, see [minilibx readme](https://github.com/42paris/minilibx-linux/blob/master/README.md) for packages to install.

## MLX for dummies

Understanding how the mlx works is central to 42's graphical projects. Here's an imperfect but quick guide:

(Quick reminder - you should link your objects with -lXext and -lX11, possibly even -lm and -lz on some systems.)

### 1. First, you have to initialize a display instance, create a window for it, and create an image.

```c
void	*mlx;
void	*win;
void	*img;

mlx = mlx_init();
win = mlx_new_window(mlx, 1920, 1080, "fdf");
img = mlx_new_image(mlx, 1920, 1080);
```

Note that these 3 functions can fail and return `NULL`. You should protect them and exit in case of error, but make sure to always cleanly destroy what you've created before exiting.

```c
mlx_destroy_image(mlx, img);
mlx_destroy_window(mlx, win);
mlx_destroy_display(mlx);
free(mlx); // for some reason, the above call doesn't free mlx, so make sure you do!
```

Before continuing, you should also store a pointer to access the image's contents, so you can write into the image later on. You should definitely create a structure for that, as we need a few other variables.

```c
typedef struct	s_img
{
	void	*img;
	char	*addr;
	int		bpp; //bits per pixel
	int		llen; //line length
	int		endian;
}	t_img;

img.addr = mlx_get_data_addr(img.img, &img.bpp, &img.llen, &img.endian);
```

The 3 extra variables are environment-specific and handled by the mlx. Their values don't really matter, you'll just need to take some of them into account when writing into the image (nothing complicated, you'll see).

### 2. To change a pixel's color, you need to write directly into the image you've created using the address you just got. You can make a function for it.

```c
void	my_pixel_put(t_img img, int x, int y, int color)
{
	*(int *)(img.addr + y * img.llen + x * img.bpp / 8) = color;
}
```

I know, this looks complicated so let's take it step by step. First, you need to offset the address by your [x, y] using the variables you got before, to reach the correct position of the pixel you're trying to write to.

Multiply y (y is the line of the pixel) by line length - you'd be tempted to multiply by whatever your window horizontal length is (like 1920) and the size of an int (4, so 1920 x 4), but `mlx_get_data_addr` gives us its own value because it *might* not be the same...

Then multiply x (x is how many pixels are before the one you're trying to change - on its own line, anyway) by how many bytes a pixel takes (which is `bits_per_pixel / 8`). This is probably 4, but, again, `mlx_get_data_addr` gave us a value because it might be different.

Then, you got the address as a `char *`, but colors are ints, so you need to convert it into `int *` before writing to it (or else you will only modify one byte, not 4).

Finally, your color should be in `0xrrggbb` hexadecimal form. As far as I can see, alpha values are ignored by the mlx (`0xaarrggbb`) so you don't need to bother.

### 3. You can change your image's pixels, but your image isn't displayed yet. Here's how to display it:

```c
my_pixel_put(img, 1920 / 2, 1080 / 2, 0xff0000); // Example use of my_pixel_put
mlx_put_image_to_window(mlx, win, img.img, 0, 0);
```

### 4. Now you'll have a nice red pixel in the center of your window. But said window disappears before you can even look at it! You need an infinite loop.

You could be tempted to create a `while (1)` loop yourself, but no need, and the mlx wouldn't work with it anyway. Instead, you use:

```c
mlx_loop(mlx);
```

Note that it's no use writing any code beyond this line as it will never be executed. Not as long as the mlx loop isn't broken out of, which shouldn't really happen anyway.

### 5. Your window stays open. But you have to hit Ctrl+C to close it. Not very nice. How do we make things happen with clicks and keypresses?

```c
mlx_hook(win, 2, 1L << 0, my_hook_function, &my_struct);
```

Here you're telling mlx that on `KeyPress` events (that's the `2` and `1L << 0`), `my_hook_function()` should fire, and the address containing `my_struct` should be passed to it.

Hence, you should have such a function somewhere, which will look like:

```c
int	my_hook_function(int key, t_mystruct *my_struct)
{
	// do very clever stuff
}
```

Once you've done that (*before* `mlx_loop`), any keyboard key pressed will cause `my_hook_function` to fire. Try putting a `printf("%d\n", key)` in there!

Note that we're passing an address of a structure there, but you can pass any address, or even *none*, if you don't particularly care. You probably will, though, if your program does anything fun.

Now this hooks the `KeyPress` event, but there are many many more! There's a `KeyRelease` one, `ButtonPress` and `ButtonRelease` (for mouse buttons), `MotionNotify` (for mouse pointer motion), and more. Go look at the XLib doc for them (or look directly at X.h linked below).

Here are a few more prototypes for different hook functions:

```c
int	mouse_hook(int button, int x, int y, void *data); // ButtonPress is 4 and 1L << 2
int	motion_hook(int x, int y, void *data); // MotionNotify is 6 and 1L << 6
int	clientmsg_hook(void *data); // 33 and 1, this fires on clicking X to close the window
```

Yes, that last one is obscure. Also, the minilibx has specific `mlx_mouse_hook` and `mlx_key_hook` functions, but you're better off using the generic one which will grant you much more freedom, even though it's slightly more difficult to use.

### 6. What `mlx_loop_hook` is and why you should use it

The essence of your graphical program will probably be to write a certain amount of pixels to your image with `my_pixel_put` before refreshing it with `mlx_put_image_to_window`. But *when* exactly should you refresh your window? After every written pixel? After an entire image is calculated? Or just a set amount of times per second? Or yet another entirely different way?

I don't believe there is any universal answer to this question, and I definitely think you should experiment with the possibilities (go crazy!). But if, like me, your original solution turns out to be quite sluggish and unenjoyable, then you are probably trying to refresh your image extremely fast, faster than your processor can handle, and you would benefit from setting a *refresh rate* (that is, running `mlx_put_image_to_window` a set amount of times per second). This is why `gettimeofday()` is allowed for graphical projects.

The first step is writing yet another hook function but this time, hooking it to the main mlx loop which fires constantly in the background.

```c
mlx_loop_hook(mlx, loop_hook, &data);
```

If you pass a function to the loop hook, instead of firing on certain events like key presses, it will fire on every iteration of the mlx loop, which is the infinite loop that starts when you use `mlx_loop`. This loop isn't really limited in how many times in runs every second. Instead, it just gets slower as you pile up operations to be done during it.

Here is a simple experiment to figure out how many times it runs per second (increment a value on every iteration, and print + reset every second):

```c
int	loop_hook(t_mystruct *data)
{
	data->count++;
	if (data->time != time(NULL))
	{
		printf("Count since last second: %d\n", data->count);
		data->time = time(NULL);
		data->count = 0;
	}
	return (0);
}
```

In my case, the loop runs about 14,000 times per second. That's pretty fast. Note that it'd run even faster without these instructions.

Now, you can try putting your `mlx_put_image_to_window` in there instead of your other event hooks, and see what happens. First, if your user experience was extremely sluggish before, there's a good chance it will suddenly get much smoother and much more enjoyable. That's because you're not piling up work for your processor faster than it can execute it. You're just telling it to work whenever it has free time.

In my case, the loop now runs 240 times per second. This is *significantly slower*, all of a sudden. This means that if I were to try running `mlx_put_image_to_window` faster than 240 times a second (which can happen very easily if I run it on a `MotionNotify` event, which fires about 600 times a second), my processor wouldn't be able to keep up and a queue would start forming.

If you wanted now to only refresh your window 60 times per second, you'd have to call `gettimeofday()` and compare its return to your last refresh's timestamp. For 60 fps, you'd want to wait 1 / 60 milliseconds, or 16,666 microseconds. You can even set a condition for it not to refresh as long as you haven't written anything new to your image.

And of course, if the operations you execute between two frames take more than 16,666 microseconds for your processor to complete, you will experience framerate drops! This is because the mlx loop only fires after your program has finished doing whatever it was doing (that is, unless you use multi-threading).

### 7. Some end notes:

I haven't talked about using xpm files (`mlx_xpm_file_to_image(void *mlx_ptr, char *filename, int *width, int *height)`) and more, as I'm still familiarizing myself with the mlx. Make sure to check out other resources and take a look at mlx.h which contains all the mlx function prototypes. Don't be afraid to try things. I suspect the mlx is so poorly documented because 42 wants us to figure it out through experimentation.

Note that the mlx does not use your gpu, only your cpu. Not sure if there's a way to let it use your gpu though!

### Thanks for reading and happy coding ^-^

## Resources

mlx - https://github.com/42paris/minilibx-linux

mlx tutorial - https://harm-smits.github.io/42docs/libs/minilibx/getting_started.html

x.h file - https://codebrowser.dev/kde/include/X11/X.h.html
