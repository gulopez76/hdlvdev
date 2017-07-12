<?php
// $Id$

/**
 * @file
 */

function uc_decimal_quantities_settings() {
  drupal_set_message(t('<strong>NOTE:</strong> this module is in early development. Currently it is only useful for products sold by the metre. In future it could potentially be used to sell products by weight. It could also potentially allow different units for different product classes.'), 'warning', FALSE);
  $form = array();
  $classes = uc_product_type_names();
  $count = count($classes);
  if ($count == 0) {
    // No product classes available.
    $form['#prefix'] = t('<p>No product classes have been created yet. <a href="@link">Create one now</a>', array(
      '@link' => url('admin/store/products/classes'),
    ));
  }
  else {
    $form['uc_decimal_quantities_product_classes'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Product classes'),
      '#options' => $classes,
      '#default_value' => variable_get('uc_decimal_quantities_product_classes', array()),
      '#description' => t('Select which product classes are able to use decimal quantities.')
    );
  }

  $form['#submit'] = array('uc_decimal_quantities_settings_submit');
  return system_settings_form($form);
}

function uc_decimal_quantities_settings_submit($form, &$form_state) {
  $form_state['values']['uc_decimal_quantities_product_classes'] = array_filter($form_state['values']['uc_decimal_quantities_product_classes']);
}