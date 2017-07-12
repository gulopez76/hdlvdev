<?php
// $Id$
/**
 * @file formdefaults.admin.inc
 * Include for formdefaults administration screen.
 */

/**
 * Search for forms matching wildcard and return all those that match.
 *
 * @param String $search_str
 * @return array  Array of forms matching search criteria and their definitions
 */
function _formdefaults_search($search_str) {
  $search_str = '%' . $search_str . '%' ;
  $result = db_query('SELECT * FROM {formdefaults_forms} WHERE formid LIKE :formid', array(':formid' => $search_str));
  $forms = array();
  foreach ($result as $form) {
    if ($form) {
       $formarray = unserialize($form->formdata);

       $forms[$form->formid]=$formarray;
    }
  }

  return $forms;

}

function _formdefaults_save_form($formid, $form_array) {
  $old_form = formdefaults_getform($formid);

  // Change the input format from d6 style to d7 style.
  // Keep until D8.
  foreach ($form_array as $key => $control) {
    if (isset($control['format']) && !isset($control['input_format'])) {
    		$form_array[$key]['input_format'] = $control['format'];
    		unset($form_array[$key]['format']);
    }
  }

  $form_data = serialize($form_array);
  if ($form_data && $form_array) {
  	if ($old_form) {
	  	db_update('formdefaults_forms')
	  	  ->condition('formid', $formid)
	  	  ->fields(array(
	  	    'formdata' => $form_data
	  	  ))
	  	  ->execute();
  	}
	  else {
	  	db_insert('formdefaults_forms')
	  	  ->fields(array(
	  	    'formid' => $formid,
	  	    'formdata' => $form_data,
	  	  ))
	  	->execute();
	  }
  }
  else {
    _formdefaults_delete_form($formid);
  }
}

function _formdefaults_delete_form($formid) {
  db_delete('formdefaults_forms')
    ->condition('formid', $formid)
    ->execute();
}



/**
 * Form management form used for inspecting and resetting forms.
 *
 * @return Form
 */
function formdefaults_manage() {
  $search_str = @$_SESSION['formdefaults_search'];
  $form['search_str'] = array(
    '#type' => 'textfield',
    '#default_value' => $search_str,
    '#description' => t('Search all forms that have a formid (name) containing the word you specify.'),
  );

  $form['search'] = array(
      '#type' => 'submit',
      '#value' => 'Search',
      '#size' => 10,
  );



  $form['results'] = array(
      '#type' => 'fieldset',
      '#title' => 'Overridden Forms',
      '#tree' => TRUE,
  );

  $form_list = _formdefaults_search($search_str);
  $list = array();
  foreach ($form_list as $form_key => $f) {
    $list[$form_key]= l($form_key, 'formdefaults/' . $form_key);
  }

  $form['results']['reset_forms'] = array('#type' => 'checkboxes',
                                          '#options' => $list);
  $form['results']['reset'] = array('#type' => 'submit',
                                    '#value' => 'Reset Selected',
                                    '#submit' => array('formdefaults_manage_reset_selected')
  );
  return $form;
}

function formdefaults_manage_submit($formid, &$form_state) {
  $form_values = $form_state['values'];
  $_SESSION['formdefaults_search'] = $form_values['search_str'];
}

function formdefaults_manage_reset_selected($formid, &$form_state) {
  $form_values = $form_state['values'];
  $_SESSION['formdefaults_search'] = $form_values['search_str'];
  if ($form_values['results']['reset_forms']) foreach ($form_values['results']['reset_forms'] as $form) if ($form) {
    _formdefaults_delete_form($form);
  }

}

function formdefaults_export() {
 $search_str = @$_SESSION['formdefaults_search'];

  $form['search_str'] = array(
    '#type' => 'textfield',
    '#default_value' => $search_str,
  );

  $form['search'] = array(
      '#type' => 'submit',
      '#value' => 'Search',
      '#size' => 10,
  );

  $form_list = _formdefaults_search($search_str);

  $form['count'] = array(
    '#type' => 'item',
    '#title' => t('Forms Exported'),
    '#value' => count((array)$form_list),
  );

  $form_data = base64_encode(serialize($form_list));
  $form['data'] = array(
      '#type' => 'textarea',
      '#title' => 'Data',
      '#default_value' => $form_data,
      '#rows' => 20,
      '#cols' => 80,
  );
  return $form;
}

function formdefaults_export_submit($formid, &$form_state) {
  $form_values = $form_state['values'];
  $_SESSION['formdefaults_search'] = $form_values['search_str'];
}

function formdefaults_data($data='') {
  static $d = '';
  if (!$d) $d = @$_SESSION['formdefaults_data'];

 // if (!$d) $d=array();
  if ($data) {
    $d = $data;
  }
  $_SESSION['formdefaults_data'] = $d;
  return $d;
}

function formdefaults_import() {
  $form_list = formdefaults_data();
  $data= '';
  if ($form_list) $data = base64_encode(serialize($form_list));

  $form['data'] = array(
        '#type' => 'textarea',
        '#title' => 'Data',
        '#default_value' => $data,
        '#rows' => 20,
        '#cols' => 80,
  );
  $form['list'] = array('#type' => 'fieldset',
                        '#title' => 'Forms');


  if ($form_list) {
    foreach ($form_list as $key => $formdata) {
      $form['list'][] = array('#type' => 'markup',
                             '#markup' => "<p>$key</p>",
                             );
    }
  }

  $form['validate'] = array(
        '#type' => 'submit',
        '#value' => 'Preview',
  );
  if ($data) {
    $form['import'] = array(
          '#type' => 'submit',
          '#value' => 'Import',
    );
  }
  return $form;
}

function formdefaults_import_submit($formid, &$form_state) {

  $form_values = $form_state['values'];
  $forms = unserialize(base64_decode($form_values['data']));
  formdefaults_data($forms);
  if ($_POST['op']=='Import') {
    foreach ($forms as $key => $formdata) {
      _formdefaults_save_form($key, $formdata);
    }
    unset($_SESSION['formdefaults_data']);
  }

}

function formdefaults_edit_form() {
  $formid = arg(1);
  $fieldname = arg(2);
    drupal_set_title(t('Edit Form @formid', array( '@formid' => $formid)));
  // Load the form
  $data = formdefaults_getform($formid);
  $fields = array();
  $form['formid'] = array(
     '#type' => 'value',
     '#value' => $formid,
     );

  foreach ($data as $f => $field) if (strpos($f, '#')!==0) {
    $t = @$field['title'] ? ' - ' . @$field['title']:'';
    $fields[$f] = l($f . $t, 'formdefaults/' . $formid . '/' . urlencode($f));
  }
  $form['fields'] = array(
     '#type' => 'checkboxes',
     '#title' => 'Overriden Fields',
     '#options' => $fields,
  );

  $form['add'] = array('#type' => 'fieldset',
                       '#title' => 'Add Fields',
                       '#collapsible' => TRUE,
                       '#collapsed' => TRUE,
  );

  $types = array('markup' => 'Markup', 'fieldset' => 'Collapsed fieldset with markup ');
  $form['add']['field_type'] = array(
     '#type' => 'select',
     '#title' => 'Type',
     '#options' => $types,
     '#description' => t('Choose Markup to add a place for instructions that are always seen.  Choose collapsed fieldset to add instructions inside an expandable box')
  );

  // Weight of
  $weight_range = range(-50, 50);
  $weights=array('unset' => 'unset');
  foreach ($weight_range as $weight) $weights[(string)$weight]=(string)$weight;

  $form['add']['weight'] = array(
    '#type' => 'select',
    '#title' => 'Weight',
    '#options' => $weights,
    '#default_value' => -49,
    '#description' => 'Controls placement within the form, -49 is a good header value or 50 is usually a good footer value',
  );

  $form['add']['add_submit'] = array(
    '#type' => 'submit',
    '#value' => 'Add',
  );


  $form['reset'] = array('#type' => 'submit', '#value' => 'Reset Selected');
  return $form;
}

function formdefaults_edit_form_submit($formid, &$form_state) {
  $form_values = $form_state['values'];
  $formid = $form_values['formid'];
  $formdef = formdefaults_getform($formid);

  // Reset fields
  if ($_POST['op'] == 'Reset Selected') {
    foreach ($form_values['fields'] as $field => $checked) {
      if ($checked) {
        unset($formdef[$field]);
      }
    }

    // Condense addon array.
    if (isset($formdef['#formdefaults_addon_fields'])) {
       $addons = (array)$formdef['#formdefaults_addon_fields'];
       $new_addons = array();

       foreach ($addons as $key => $field) {
         if (@$formdef[$key]) {
           $i = 'formdefaults_' . count($new_addons);

           if ($i != $key) {
             $formdef[$i] = $formdef[$key];
             unset($formdef[$key]);
             if ($formdef[$i . '_markup']) {
               $formdef[$i . '_markup'] = $formdef[$key . '_markup'];
               unset($formdef[$key . '_markup']);
             }
           }
           $new_addons[$i] = $field;
         }
       }
       $formdef['#formdefaults_addon_fields'] = $new_addons;
    }

  }


  if ($_POST['op'] == 'Add') {
    $i = count((array)@$formdef['#formdefaults_addon_fields']);
    $key = 'formdefaults_' . $i;
    $subkey = $key . '_markup';
    $field = array();
    $weight = $form_values['weight'];
    switch ($form_values['field_type']) {
      case "markup":
        $field = array('#type' => 'markup', '#markup' => '' );

        $formdef[$key] = array('type' => 'markup', 'value' => '<p>Replace with your own markup</p>', 'format' => 0, 'weight' => $weight);
        break;
      case "fieldset":
        $field = array('#type' => 'fieldset',
                       '#title' => 'Untitled',
                       '#collapsible' => TRUE,
                       '#collapsed' => TRUE,
                       $subkey => array('#type' => 'markup', '#value' => ''),
                       );
        $formdef[$key] = array('type' => 'fieldset', 'title' => 'Untitled', 'weight' => $weight);
        $formdef[$subkey] = array('type' => 'markup', 'value' => '<p>Replace with your own markup</p>');
        break;

    }
    $formdef['#formdefaults_addon_fields'][$key] = $field;
  }
  _formdefaults_save_form($formid, $formdef);

}

function formdefaults_filter_element($fmt) {

  global $user;
  $element['format'] =  array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Input formats'),
  );
    // Get a list of formats that the current user has access to.
  $formats = filter_formats($user);
  foreach ($formats as $format) {
    $options[$format->format] = $format->name;
    $element['format']['guidelines'][$format->format] = array(
      '#theme' => 'filter_guidelines',
      '#format' => $format,
    );
  }
  $element['format']['guidelines']['#weight']=12;

  $element['format']['input_format'] = array(
    '#type' => 'select',
    '#title' => t('Text format'),
    '#options' => $options,
    '#default_value' => $fmt,
    '#access' => count($formats) > 1,
    '#weight' => 10,
    '#attributes' => array('class' => array('filter-list')),
  );

  $element['format']['help'] = array(
    '#type' => 'container',
    '#theme' => 'filter_tips_more_info',
    '#attributes' => array('class' => array('filter-help')),
    '#weight' => 11,
  );
  return $element['format'];
}


/**
 * Form to edit the field title and description.
 */
function formdefaults_edit_field() {
  $form_array = $_SESSION['formdefaults_forms'];
  $formid = arg(1);
  $fieldname = arg(2);
  drupal_set_title(t('Edit field @fieldname in @formid', array( '@fieldname' => $fieldname, '@formid' => $formid)));

  $originalfields = @$form_array[$formid][$fieldname]? $form_array[$formid][$fieldname]:array();
  $savedform = formdefaults_getform($formid);
  $weight_range = range(-50, 50);
  $weights=array('unset' => 'unset');

  foreach ($weight_range as $weight) $weights[(string)$weight]=(string)$weight;

  if (is_array(@$savedform[$fieldname])) $formfields = array_merge($originalfields, @$savedform[$fieldname]);
  else $formfields = $originalfields;
  $type = $formfields['type'];
  if (!$type) {
    if (isset($formfields['input_format'])) $type = 'markup';
  }
  if (@$originalfields['type']) $type = $originalfields['type'];

  $form['formid'] = array(
     '#type' => 'value',
     '#value' => $formid,
     );

  $form['fieldname'] = array(
     '#type' => 'value',
     '#value' => $fieldname,
     );

  $form['type'] = array(
     '#type' => 'value',
     '#title' => 'Field Type',
     '#value' => $type,
     );

  $form['warning'] = array(
     '#type' => 'markup',
     '#value' => 'Some text to edit',
     );

  $form['hide_it'] = array(
     '#type' => 'checkbox',
     '#title' => 'Hide this field',
     '#description' => 'Checking this box will convert the field to a hidden field.' .
         ' You will need to use the edit form link to unhide them.',
     '#default_value' => @$formfields['hide_it']
  );


  if ($type == 'markup') {
    $form['value'] = array(
       '#type' => 'text_format',
       '#title' => 'Text or markup',
       '#rows' => 30,
       '#cols' => 80,
       '#format' => @$formfields['input_format'],
       '#default_value' => @$formfields['value'],
       );

    $form['value_original'] = array(
      '#type' => 'item',
      '#title' => 'Original value',
      '#value' => @$originalfields['value'],
      );
  }
  else {
    $form['title'] = array(
       '#type' => 'textfield',
       '#title' => 'Field Title',
       '#default_value' => @$formfields['title'],
       );

    $form['title_old'] = array(
       '#type' => 'item',
       '#title' => 'Original Title',
       '#value' => @$originalfields['title'],
       );

    $form['description'] = array(
       '#type' => 'textarea',
       '#title' => 'Field Description',
       '#default_value' => $formfields['description'],
       '#rows' => 30,
       '#cols' => 80,
       );

    $form['description_old'] = array(
       '#type' => 'item',
       '#title' => 'Original Description',
       '#value' => $originalfields['description'],
       );

  }
  if ($type == 'fieldset' ) {
    $truefalse = array('' => 'Leave alone', TRUE => 'Yes', FALSE => 'No');
    $form['collapsible'] = array(
      '#type' => 'radios',
      '#title' => 'Collapsible',
      '#options' => $truefalse,
      '#default_value' => @$formfields['collapsible'],
    );

    $form['collapsed'] = array(
      '#type' => 'radios',
      '#title' => 'Collapsed',
      '#options' => $truefalse,
      '#default_value' => @$formfields['collapsed'],
    );
  }
  $form['weight'] = array(
       '#type' => 'select',
       '#title' => 'Weight',
       '#options' => $weights,
       '#default_value' => @$formfields['weight'],
       '#description' => 'Higher values appear near at the top of the form, lower values at the bottom.',
       );
  $form['weight_old'] = array(
       '#type' => 'item',
       '#title' => 'Original Weight',
       '#value' => @$originalfields['weight'],
       );

  $form['submit'] = array(
     '#type' => 'submit',
     '#value' => 'Save',
     );

  $form['reset'] = array(
     '#type' => 'submit',
     '#value'  => 'Reset',
  );
  return $form;
}

function formdefaults_edit_field_submit($formid, &$form_state) {
  $form_values = $form_state['values'];
  $formid = $form_values['formid'];
  $fieldname = $form_values['fieldname'];
  $formarray=formdefaults_getform($formid);
  $baseform = $formarray;
  // set the form values
  if ($_POST['op']=='Reset') {
    unset($formarray[$fieldname]);
  }
  else {
    if ($form_values['type'] == 'markup') {
      $formarray[$fieldname]['value'] = $form_values['value']['value'];
      $formarray[$fieldname]['input_format'] = $form_values['value']['format'];
    }
    else {
      $formarray[$fieldname]['title'] = $form_values['title'];
      $formarray[$fieldname]['description'] = $form_values['description'];
    }
    if (@$form_values['collapsible'] === '') {
      unset($formarray[$fieldname]['collapsible']);
    }
    else {
      $formarray[$fieldname]['collapsible'] = @$form_values['collapsible'];
    }
    if (@$form_values['collapsed'] === '') {
      unset($formarray[$fieldname]['collapsed']);
    }
    else {
      $formarray[$fieldname]['collapsed'] = @$form_values['collapsed'];
    }
    $formarray[$fieldname]['hide_it'] =$form_values['hide_it'];
    $formarray[$fieldname]['weight'] = $form_values['weight'];
    $formarray[$fieldname]['type'] = $form_values['type'];
  }
  _formdefaults_save_form($formid, $formarray);
  $form_state['redirect'] = 'formdefaults/' . $formid;
}
