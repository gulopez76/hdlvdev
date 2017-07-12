<?php

namespace Drupal\autoload\Tests\Unit;

/**
 * Class ExtensionsTest.
 */
class ExtensionsTest extends AuxiliaryTest {

  /**
   * {@inheritdoc}
   */
  protected static $modules = array('autoload_test_extensions');

  /**
   * {@inheritdoc}
   */
  public static function getInfo() {
    $info = parent::getInfo();
    $info['name'] = 'Extensions';
    $info['description'] = t('Ensure that registered file extensions available for autoloading.');

    return $info;
  }

  /**
   * {@inheritdoc}
   */
  public function test() {
    // These extensions are always available!
    $this->assertExtensions(array('.php', '.inc'));

    // Only Drupal can change this. Modules cannot!
    spl_autoload_extensions('.test');

    // List of extensions changed as expected.
    $this->assertExtensions(array('.php', '.inc', '.test'));
    // But class must not exist since extension was registered not by Drupal.
    $this->assertFalse(class_exists('Drupal\autoload_test_extensions\PSR4'));
  }

}
