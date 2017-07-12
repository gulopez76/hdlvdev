<?php

namespace Drupal\autoload\Tests\Unit;

/**
 * Class DrupalTest.
 */
class DrupalTest extends AuxiliaryTest {

  /**
   * {@inheritdoc}
   */
  protected static $modules = array('autoload_test_drupal');

  /**
   * {@inheritdoc}
   */
  public static function getInfo() {
    $info = parent::getInfo();
    $info['name'] = 'Drupal';
    $info['description'] = t('Testing functionality of autoload Drupal-way namespaces.');

    return $info;
  }

  /**
   * {@inheritdoc}
   */
  public function test() {
    // If something will not work we'll get fatal error :)
    new \Drupal\autoload_test_drupal\PSR0();
    new \Drupal\autoload_test_drupal\PSR4();
  }

}
