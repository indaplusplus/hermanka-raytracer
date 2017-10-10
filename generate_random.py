from random import randint

image_width, image_height = 1800, 1300
eye_pos = (0, 0, -2000)

n_lights = randint(5, 20)
lights = [(randint(-4000, 4000), randint(-4000, 4000), randint(-4000, 4000)) for _ in range(n_lights)]

def generate_sphere():
    return (randint(-4000, 4000), randint(-4000, 4000), randint(0, 4000),
            randint(0,400), randint(0,255), randint(0,255), randint(0,255), 1)
n_spheres = randint(20, 60);
spheres = [generate_sphere() for _ in range(n_spheres)]

print(image_width, image_height)
print(*eye_pos)
print(n_lights)
for i in lights: print(*i)
print(n_spheres)
for i in spheres: print(*i)

