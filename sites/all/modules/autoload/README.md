# Autoload

Want to not care about loading classes, traits or interfaces? Aim of this development - is what you are looking for!

## Usage

You able to use one or both of known autoloading standards.

### Drupal way

Open `*.info` file and add `autolaod = TRUE` there. Now just place the objects into `lib/Drupal/YOUR_MODULE_NAME` or `src`.

```ini
; All namespaces must be followed by "Drupal\YOUR_MODULE_NAME".
autoload = TRUE
```

Have a look at [tests](tests/autoload_test_drupal) as an example.

### Custom namespaces

Open `*.info` file and configure the `autoload` directive, where keys are subdirectories inside of the module directory and values - are namespaces. As much as needed directories and namespace bases could be added by such way.

#### PSR-0

```ini
; All objects, namespace path of which starts from "CTools",
; will be searched inside of "<MODULE_PATH>/psr-0".
autoload[psr-0][] = CTools
```

#### PSR-4

```ini
; All objects, namespace path of which starts from "CTools\Plugins",
; will be searched inside of "<MODULE_PATH>/psr-4".
autoload[psr-4][] = CTools\Plugins
```

Take into account the trailing slash! It must be at the end of global namespace to use `PSR-4` standard. It looks similar to `PSR-0`, but that slash telling that it's not true.

```ini
; All objects, namespace path of which starts from "CTools",
; will be searched inside of "<MODULE_PATH>/psr-4".
autoload[psr-4][] = CTools\
```

Have a look at [tests](tests/autoload_test_custom) as an example.
