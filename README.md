# Mousey

Mousey is a free and open-source framework that you can use to create good and awesome 2D games using the Squirrel language. Right now on its very first premice, but you can code simple games like Pong or even Asteroids (and that is cool).

## Haven't get in touch with Squirrel before?

It's a really cool language. If you know Lua and object-oriented programming, you shouldn't be lost. There are some good resources about it, including the [documentation](http://www.squirrel-lang.org/squirreldoc/squirrel3.pdf).

## Dependencies

- SDL2
- SDL2_image
- SDL2_ttf
- SDL2_sound
- OpenGL 3.1
- OpenAL
- libyaml
- SCons
- Squirrel

## To do

- [ ] Music streaming support
- [ ] Physics engine implementation (we'll use Chipmunk to keep it rodent-themed)
- [ ] Joystick/Gamepad support
- [ ] Networking
- [ ] Documentation
- [ ] 3D support (will not likely be here in the first version of the framework)

## Here's what you can do

### Drawing text

```squirrel
function render() {
  Mousey.draw_text("Hello World!", Mousey.Vector2(400, 300))
}
```

### Draw image

```squirrel
local img

function initialize() {
  img = Mousey.Texture("mouse.png")
}

function render() {
  img.draw(Mousey.Vector2(200, 200))
}
```

### Play sound

```squirrel
local snd

function initialize() {
  snd = Mousey.Sound("squeak.wav")
}

function update(dt) {
  if(Mousey.Keyboard.is_pressed(Mousey.Key.P)) {
    snd.play()
  }
}
```
