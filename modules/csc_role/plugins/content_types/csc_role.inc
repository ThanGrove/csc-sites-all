<?php

$plugin = array(
  'single' => TRUE,
  'title' => t('CSC Personnel'),
  'description' => t('Shows CSC Personnel by their role'),
  'category' => t('CSC Plugins'),
  'render callback' => 'csc_role_render',
  'edit form' => 'csc_role_edit_form',
  'admin info' => 'csc_role_info',
  'defaults' => array(
		'role' => 0,
	),
);

/**
 * 'admin info' callback for panel pane.
 */
function csc_role_info($subtype, $conf, $contexts) {
  if (!empty($conf)) {
    $block = new stdClass;
    $block->title = 'The CSC Personnel';
    $block->content = 'CSC Role Widget';
    return $block;
  }
}



/**
 * 'Edit form' callback for the content type.
 */

function csc_role_edit_form($form, &$form_state) {

  $conf = $form_state['conf'];
	
	$form['role'] = array(
       '#type' => 'select',
       '#title' => t('Role'),
       '#options' => array(
          0 => t('None'),
         	1 => t('Director'),
         	2 => t('Staff'),
       ),
       '#default_value' => (empty($conf['role'])) ? 0 : $conf['role'],
       '#description' => t('Choose the CSC role you want to list in this pane'),
  );
  return $form;

}

/**
 * The submit form stores the data in $conf.
 */
 
function csc_role_edit_form_submit($form, &$form_state) {
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    if (isset($form_state['values'][$key])) {
      $form_state['conf'][$key] = $form_state['values'][$key];
    }
  }
}

/**
 * Build blocks for Related events
 */
function csc_role_render($subtype, $conf, $panel_args, $context = NULL) {
	$role = $conf['role'];
	$qry = "SELECT n.nid FROM {node} n, {field_data_field_csc_role} fsd WHERE n.status=1 AND n.type = 'people' AND n.nid = fsd.entity_id AND fsd.field_csc_role_value=:role";
  $r = db_query($qry, array(':role' => $role));
	
  $output = '';
  foreach($r as $row){
    $node = node_load($row->nid);
    $lng = $node->language;

    $title = $node->title;

    $output.= theme('csc_role_item', array('title' => $title,
                                                'nid' => $node->nid,
                                                'position' => isset($node->field_position[$lng][0]['value']) ? $node->field_position[$lng][0]['value'] : '',
                                                'image' => isset($node->field_thumbnail[$lng][0]['uri']) ? $node->field_thumbnail[$lng][0]['uri'] : ''
                                                //'image' => isset($node->field_featured_image[$lng][1]['uri'])? $node->field_featured_image[$lng][1]['uri'] : '',
                                                ));
  }

  $block = new stdClass();
  $block->title = 'The CSC Personnel';
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
