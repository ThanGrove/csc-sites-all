<?php

$plugin = array(
  'single' => TRUE,
  'title' => t('CSC Role'),
  'description' => t('Shows CSC Role'),
  'category' => t('CSC Plugins'),
  'render callback' => 'csc_role_render',
  'admin info' => 'csc_role_info',
  'settings form' => 'csc_role_ctools_plugin_settings',
  'settings form validation' => 'csc_role_ctools_plugin_settings_validate',
  'settings form submit' => 'csc_role_ctools_plugin_settings_submit',
  //'summary' => 'csc_role_ctools_plugin_summary',
);

/**
 * 'admin info' callback for panel pane.
 */
function csc_role_info($subtype, $conf, $contexts) {
  if (!empty($conf)) {
    $block = new stdClass;
    $block->title = 'The CSC Role';
    $block->content = 'CSC Role Widget';
    return $block;
  }
}

/**
 * Settings form for the 'term depth' access plugin.
 */
function csc_role_ctools_plugin_settings($form, &$form_state, $conf) {
}

/**
 * Settings form for the 'term depth' access plugin.
 */
function csc_role_ctools_plugin_settings_validate($form, &$form_state, $conf) {
	
	return true;
}


/**
 * Submit function for the access plugins settings.
 *
 * We cast all settings to numbers to ensure they can be safely handled.
 */
function csc_role_ctools_plugin_settings_submit($form, $form_state) {
  foreach (array('depth', 'vid') as $key) {
    $form_state['conf'][$key] = (integer) $form_state['values']['settings'][$key];
  }
}

/**
 * Build blocks for Related events
 */
function csc_role_render($subtype, $conf, $panel_args, $context = NULL) {
 // TODO: Need to add field with name csc_role and change this here!
  $qry = "SELECT n.nid FROM {node} n, {field_data_field_csc_director} fsd WHERE n.status=1 AND n.type = 'people' AND n.nid = fsd.entity_id AND fsd.field_csc_director_value=1";
  $r = db_query($qry);

  $output = '';
  foreach($r as $row){
    $node = node_load($row->nid);
    $lng = $node->language;

    $title = $node->title;
    //$lastname = $node->title;
    //$firstname = $node->field_first_name[$lng][0]['value'];
    //$middlename = $node->field_middle_name[$lng][0]['value'];

    //$title =  "$firstname ";
    //if($middlename) $title .= "$middlename ";
    //$title .= $lastname;

    $output.= theme('csc_role_item', array('title' => $title,
                                                'nid' => $node->nid,
                                                'position' => isset($node->field_position[$lng][0]['value']) ? $node->field_position[$lng][0]['value'] : '',
                                                'image' => isset($node->field_thumbnail[$lng][0]['uri']) ? $node->field_thumbnail[$lng][0]['uri'] : ''
                                                //'image' => isset($node->field_featured_image[$lng][1]['uri'])? $node->field_featured_image[$lng][1]['uri'] : '',
                                                ));
  }

  $block = new stdClass();
  $block->title = 'The CSC role';
  $block->content = '<div class="csc-role-container">' . $output . '</div>';

  return $block;
}

/**
 * Theme function
 */
function theme_csc_role_item($var){
  //$url = l($var['title'], "node/" . $var['nid']);
  $image = ($var['image']) ? theme_image_style(array('style_name' => 'csc_medium','path' => $var['image'], 'alt' => $var['title'], 'title' => $var['title'], 'width' => '', 'height' => ''  )) : '';

  $output = '<div class="csc-role-wrapper">';
  $outputInside = '<div class="csc-role-detail">';
  $outputInside.= $image;
  $outputInside.= '<div class="csc-role-text">';
  $outputInside.= "<h2>" . $var['title'] . "</h2>";
  $outputInside.= $var['position'];
  $outputInside.= '</div></div>';
  $output.= l($outputInside, "node/" . $var['nid'], array('html' => true)) . '</div>';

  return $output;
}
