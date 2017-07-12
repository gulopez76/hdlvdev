<?php

namespace Drupal\autoload\Tests\Unit;

/**
 * Class CustomTest.
 */
class CustomTest extends AuxiliaryTest {

  /**
   * {@inheritdoc}
   */
  protected static $modules = array('autoload_test_custom');

  /**
   * {@inheritdoc}
   */
  public static function getInfo() {
    $info = parent::getInfo();
    $info['name'] = 'Custom';
    $info['description'] = t('Testing functionality of autoload custom namespaces.');

    return $info;
  }

  /**
   * {@inheritdoc}
   */
  public function test() {
    // If something will not work we'll get fatal error :)
    new \Autoload\Tests\PSR0();
    new \Autoload\Tests\PSR4();
    new \AutoloadTests\PSR4();
    new \Autoload\Tests\Example\Test();
  }

}
