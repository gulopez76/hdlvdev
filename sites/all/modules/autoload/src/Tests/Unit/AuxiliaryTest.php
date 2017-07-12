<?php

namespace Drupal\autoload\Tests\Unit;

/**
 * Class AuxiliaryTest.
 */
class AuxiliaryTest extends \DrupalWebTestCase {

  /**
   * Modules which should enabled for testing.
   *
   * @var string[]
   */
  protected static $modules = array('autoload');

  /**
   * {@inheritdoc}
   */
  public static function getInfo() {
    return array(
      'name' => 'Auxiliary',
      'group' => 'Autoload',
      'description' => t('Testing various auxiliary functions.'),
    );
  }

  /**
   * {@inheritdoc}
   */
  protected function setUp() {
    parent::setUp(static::$modules);
  }

  /**
   * Unit tests for auxiliary functions.
   */
  public function test() {
    $this->assertEqual(autoload_namespace_to_path('One\Two/Three'), 'One/Two/Three');
    $this->assertEqual(autoload_path_to_namespace('One/Two\Three'), 'One\Two\Three');
  }

  /**
   * Assert sought classes.
   *
   * @param string $namespace
   *   Namespace of classes to seek for.
   * @param string[] $classes
   *   Classes list.
   */
  protected function assertClasses($namespace, array $classes) {
    $this->assertFalse(array_diff(autoload_seek_classes($namespace), $classes), 'Required classes found.');
  }

  /**
   * Assert registered extensions for autoloading.
   *
   * @param string[] $extensions
   *   Extensions list. Each one must start from dot.
   */
  protected function assertExtensions(array $extensions) {
    $this->assertFalse(array_diff(autoload_extensions(), $extensions), 'Required extensions available.');
  }

}
