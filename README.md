# GoodRTS

GoodRTS is a Data-Driven RTS (Real-Time Strategy) game engine. The engine is managed via Lua and provides its own simple file extensions to handle templates, assets, and Lua bindings.
Through its custom file extensions (`CHC`, `PSA`, `ENT`, `TLP`), it allows you to control bindings, asset/script management, and entity templates.

## File Extensions

* **CHC (Cache):** Reads file paths grouped together to bind resources to the cache. This includes Graphics (PNG, BMP), Audio (WAV), Animations (PSA), Scripts (LUA), Entity Templates (ENT), Tile Pack Templates (TLP), and others.
* **PSA (PathS Animation):** Used to store sprites grouped by their graphic file paths.
* **ENT (Entity):** Assigns the base creation values and properties for entities (game objects).
* **TLP (Tile Pack):** Packages map tiles into a single file and stores their base values (it is recommended to link 25-frame animations).

## World Lua APIs

### Entity Management via Lua
In the functions below, `X` represents the Lua prefix defined for the entity inside its ENT file (e.g., `TreeSpawn`).

* **`XSpawn`**: The most basic function, called only once when the entity spawns into the world.
* **`XDied`**: Called once when the entity's state changes to 'dead'.
* **`XThink`**: The main loop function, called continuously during the game loop with a delay specified by the author.
* **`XMove`**: Called once when the entity receives a move order.
* **`XStop`**: Called once when the entity finishes or stops an order.
* **`XAroundMe`**: Called once when another entity (that this unit is focusing on) enters its attack range.
* **`XAttack`**: Called continuously with a specified delay, as long as the target entity remains in its attack range.
* **`XFollow`**: Called continuously as long as the entity is chasing its target.
* **`XTouch`**: Called when the entity collides with another entity.
* **`XSaw`**: A fixed-delay function called continuously as long as another entity remains within its field of view.

### Basic Function Parameters: `(self, globaltime, dt)`
* **`self`**: The core parameter representing the entity itself.
* **`globaltime`**: Real-time data distributed to the World and GUI by the Game Class. It updates as long as the game window is open and is highly important for calculating delays.
* **`dt` (Delta Time)**: The time difference calculated at the exact moment the function is called.

### Special Purpose Parameters: `(target, distance)`
* **`target`**: Represents the target location for `XMove` and `XStop`. For `XAroundMe`, `XAttack`, `XFollow`, `XTouch`, and `XSaw`, it represents the Target Entity.
* **`distance`**: Used in `XSaw` to provide the exact distance between the entity and the target that just entered its vision.

## Accessing, Assigning, and Reading Entity Values

You can access an entity using the `self` parameter, which is included as the first parameter in all functions called for entities.

In Lua, you can only access the following groups and data:
`(eClass, id, ownerID, states, transform, graphic, logic, nowAction(order type only))`

#### Basic Identifiers
* `self.id` *(Read-only)*
* `self.ownerID = ?`
* `self.eClass = ?`
* `self.nowAction` *(Read-only, retrieves the current order type)*

#### States (True/False)
* `self.states_isSelected = ?`
* `self.states_dontMove = ?`
* `self.states_canItTouch = ?`
* `self.states_canItSee = ?`
* `self.states_interactElev = ?`
* `self.states_isNoticed = ?`
* `self.states_isCollison = ?`

#### Transform (Position, Size, Rotation)
* `self.transform_origin_x` / `origin_y = ?`
* `self.transform_direction = ?`
* `self.transform_size = ?`
* `self.transform_angles = ?`

#### Logic (Stats and Timers)
* `self[0] = ?` *(Note: This accesses a dynamic_vector. It allows entities to store their own custom internal variables. For example, `self[0] = 5` writes the value 5 into index 0. There is no element limit for this list other than your operating system's memory).*
* `self.logic_attackRadius = ?`
* `self.logic_maxDamage = ?`
* `self.logic_minDamage = ?`
* `self.logic_hitPoint = ?`
* `self.logic_moveSpeed = ?`
* `self.logic_nexttime = ?` *(Next call time for the XThink function)*
* `self.logic_nexttimeAttack = ?` *(Next call time for the XAttack function)*

## Global World APIs
* **`PlayAnimation(id, animation)`** : Plays the current animation by one frame.
* **`PlayAnimation(id, animation, setframe)`** : Sets the current animation to a specific frame.
* **`GiveOrder(id, order)`** : Issues a generic order to the entity matching the given ID.
* **`GiveOrder(id, "ATTACK", target)`** : Issues an attack order to the entity matching the ID, targeting another entity.
* **`PlaySound(id, index)`** : Plays a specific sound from the entity's sound list based on the index number.
* **`MakeEntity(template, origin_x, origin_y, ownerID)`** : Spawns a new entity at the specified coordinates, using the chosen template, and assigns it to the player matching the ownerID.

## GUI Lua APIs

Provides an event-driven and data-driven infrastructure for GUI management.
UI elements are created via Lua using `MakeWidget`, allowing windows and buttons to be easily linked together.

### Widget Types (Forms)
* **`WINDOW`**: A base window template that carries other UI elements.
* **`BUTTON`**: A clickable button.
* **`SLIDER`**: A scroll/slider bar.
* **`TEXTBOX`**: A text input area.
* **`LIST`**: A dynamic row/text list template.
* **`CHECKBOX`**: A toggleable check box.

### GUI Management via Lua
In the functions below, `X` represents the Lua prefix defined inside `MakeWidget` (e.g., `StartBtnFirstTime`).

* **`XFirstTime`**: The base function called only once when the widget is first created.
* **`XThink`**: The main loop function, called every frame as long as the widget is open and hasn't been deleted.
* **`XClick`**: Called whenever the widget is clicked with the mouse.

### Basic GUI Function Parameters: `(self, dt)` and `(self, click, dt)`
* **`self`**: The core parameter representing the widget itself.
* **`dt`**: The time difference calculated at the moment the function is called.
* **`click`**: Indicates the click type (1 = Left Click, 2 = Right Click).

## Accessing, Assigning, and Reading Widget Values

Just like entities, widgets are accessed via the `self` parameter.

#### States
* `self.state_is_active = ?` *(Boolean)*
* `self.state_is_firsttime` *(Read-only - Boolean)*
* `self.state_is_touch_mouse` *(Read-only - Boolean)*
* `self.states_closed = ?` *(Boolean. Note: When set to true, the widget and all of its attached sub-widgets are deleted).*

#### Transform (Position and Scale)
* `self.transform_origin_x = ?`
* `self.transform_origin_y = ?`
* `self.transform_scale_x = ?`
* `self.transform_scale_y = ?`
* `self.transform_size = ?`
* `self.transform_angle = ?`

#### Visual (Colors and Text)
* `self.visual_color = {r, g, b, a}`
* `self.visual_string = ?` *(String. Note: This represents the text visible on the screen or the window title).*

#### Logic (Lists and Custom Data)
* `self.logic_strvalues_push = ?` *(String. Note: Adds a new row/text to a LIST widget).*
* `self.logic_strvalues_pop = ?` *(Number. Note: Removes the specified amount of rows from a LIST widget).*
* `self[0] = ?` *(Note: Just like entities, this accesses the dynamic_vector. It is used to store custom numeric data directly inside the widget).*

## Global GUI APIs
* **`MakeWidget("FORM", origin_x, origin_y, scale_x, scale_y, "luaprefix", ownerID/Title)`** : Creates a new widget and returns its ID.
  * *Example (Window):* `local winId = MakeWidget("WINDOW", 400, 200, 300, 200, "MyWin", "Window Title")`
  * *Example (Button):* `MakeWidget("BUTTON", 50, 80, 200, 40, "StartBtn", winId)`
* **`GetDir("path")`** : Returns `.MAP` (map files) in the specified folder as a Lua table (array).
  * *Example:* `local maps = GetDir("maps")`
* **`GetWidget(id)`** : Grants direct access to the widget object that matches the specified ID.
