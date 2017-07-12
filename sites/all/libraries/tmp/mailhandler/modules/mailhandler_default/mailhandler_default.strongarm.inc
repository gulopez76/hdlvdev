<?php
/**
 * @file
 * Strongarm variable overrides.
 */

/**
 * Implementation of hook_strongarm().
 */
function mailhandler_default_strongarm() {
  $export = array();

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_mailhandler_source';
  $strongarm->value = 0;
  $export['comment_anonymous_mailhandler_source'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_controls_mailhandler_source';
  $strongarm->value = '3';
  $export['comment_controls_mailhandler_source'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_mailhandler_source';
  $strongarm->value = '4';
  $export['comment_default_mode_mailhandler_source'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_order_mailhandler_source';
  $strongarm->value = '1';
  $export['comment_default_order_mailhandler_source'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_mailhandler_source';
  $strongarm->value = '50';
  $export['comment_default_per_page_mailhandler_source'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_mailhandler_source';
  $strongarm->value = '0';
  $export['comment_form_location_mailhandler_source'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_mailhandler_source';
  $strongarm->value = '0';
  $export['comment_mailhandler_source'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_mailhandler_source';
  $strongarm->value = '1';
  $export['comment_preview_mailhandler_source'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_mailhandler_source';
  $strongarm->value = '1';
  $export['comment_subject_field_mailhandler_source'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_mailhandler_source';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_mailhandler_source'] = $strongarm;

  return $export;
}
