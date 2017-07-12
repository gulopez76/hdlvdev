<?php
/**
 * @file
 * Feature provides a default Mailhandler importer to produce nodes from emails.
 */

/**
 * Implementation of hook_ctools_plugin_api().
 */
function mailhandler_default_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "feeds" && $api == "feeds_importer_default") {
    return array("version" => 1);
  }
  elseif ($module == "mailhandler" && $api == "mailhandler_mailbox") {
    return array("version" => 2);
  }
  elseif ($module == "strongarm" && $api == "strongarm") {
    return array("version" => 1);
  }
}

/**
 * Implementation of hook_node_info().
 */
function mailhandler_default_node_info() {
  $items = array(
    'mailhandler_source' => array(
      'name' => t('Mailhandler source'),
      'module' => 'features',
      'description' => t('A source node for a Mailhandler importer, allowing for automatic retrieval on cron.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '0',
      'body_label' => '',
      'min_word_count' => '0',
      'help' => '',
    ),
  );
  return $items;
}
