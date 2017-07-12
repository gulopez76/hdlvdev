<?php

/**
 * @file
 * Ubercart Ordered Products reports administration.
 */

/**
 * General settings for the Ubercart Ordered Products reports.
 *
 * @see uc_op_reports_settings_form_validate()
 * @ingroup forms
 */
function uc_op_reports_settings_form($form, &$form_state) {
  $form['buttons']['#weight'] = 10;

  $form['uc_op_reports'] = array(
    '#type' => 'fieldset',
    '#title' => t('Ordered products reports settings'),
    '#collapsed' => TRUE,
    '#collapsible' => TRUE,
    '#weight' => 5,
  );
  $form['uc_op_reports']['uc_op_reports_user_tab_title'] = array(
    '#type' => 'radios',
    '#title' => t('Title of ordered products tab on user profiles'),
    '#options' => uc_op_reports_user_tab_titles(),
    '#default_value' => variable_get('uc_op_reports_user_tab_title', 'ordered_products'),
  );
  $form['uc_op_reports']['uc_op_reports_product_classes'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Included product classes'),
    '#description' => t('Only products of the selected classes will appear on ordered products reports.'),
    '#options' => uc_product_type_names(),
    '#default_value' => variable_get('uc_op_reports_product_classes', array('product')),
  );

  $form['uc_op_reports']['fields'] = array(
    '#theme' => 'uc_op_reports_fields',
  );

  // Loop through all the available ordered products reports fields.
  foreach (uc_op_reports_fields() as $key => $value) {
    $form['uc_op_reports']['fields'][$key] = array(
      '#weight' => variable_get('uc_opr_field_' . $key . '_weight', $value['weight']),
    );
    $form['uc_op_reports']['fields'][$key]['field_name'] = array(
      '#markup' => $value['title'],
    );
    $form['uc_op_reports']['fields'][$key]['uc_opr_field_' . $key . '_weight'] = array(
      '#type' => 'weight',
      '#delta' => 10,
      '#default_value' => variable_get('uc_opr_field_' . $key . '_weight', $value['weight']),
    );
    $form['uc_op_reports']['fields'][$key]['uc_opr_field_' . $key . '_customer'] = array(
      '#type' => 'checkbox',
      '#default_value' => variable_get('uc_opr_field_' . $key . '_customer', $value['customer']),
    );
    $form['uc_op_reports']['fields'][$key]['uc_opr_field_' . $key . '_admin'] = array(
      '#type' => 'checkbox',
      '#default_value' => variable_get('uc_opr_field_' . $key . '_admin', $value['admin']),
    );
  }
  return system_settings_form($form);
}

/*
 * Theme the table on the settings forms for the fields in the reports.
 */
function theme_uc_op_reports_fields($variables) {
  $form = $variables['form'];
  $header = array(t('Report field'), t('Weight'), t('Customer'), t('Admin'));

  $rows = array();

  foreach (element_children($form) as $field) {
    $rows[] = array(
      drupal_render($form[$field]['field_name']),
      drupal_render($form[$field]['uc_opr_field_' . $field . '_weight']),
      drupal_render($form[$field]['uc_opr_field_' . $field . '_customer']),
      drupal_render($form[$field]['uc_opr_field_' . $field . '_admin']),
    );
  }

  return theme('table', array('header' => $header, 'rows' => $rows));
}
