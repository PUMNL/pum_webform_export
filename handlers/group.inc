<?php

class PumWebformExportGroupHandler {
    
  public function __construct() {
  }
  
  public static function export(&$node, $original_node) {
    $e = new PumWebformExportGroupHandler();
    $node->webform_civicrm = $e->checkCiviElements($node->webform_civicrm);
    $node->webform['components'] = $e->checkComponents($node->webform['components']);
  }
  
  public static function import(&$node, $original_node) {
    $e = new PumWebformExportGroupHandler();
    $node->webform_civicrm = $e->importCheckCiviElements($node->webform_civicrm);
    $node->webform['components'] = $e->importCheckComponents($node->webform['components']);
  }
  
  public function checkComponents(&$array) {
    $return = array();
    foreach($array as $key => $element) {
      if (isset($element['extra']['filters']['group']) && is_array($element['extra']['filters']['group'])) {
        $element['extra']['filters']['group'] = $this->exportGroup($element['extra']['filters']['group']);
      }
      if (isset($element['extra']['filters']['tag']) && is_array($element['extra']['filters']['tag'])) {
        $element['extra']['filters']['tag'] = $this->exportTag($element['extra']['filters']['tag']);
      }
      
      $return[$key] = $element;
    }
    return $return;
  }
  
  public function importCheckComponents(&$array) {
    $return = array();
    foreach($array as $key => $element) {
      if (isset($element['extra']['filters']['group']) && is_array($element['extra']['filters']['group'])) {
        $element['extra']['filters']['group'] = $this->importGroup($element['extra']['filters']['group']);
      }
      if (isset($element['extra']['filters']['tag']) && is_array($element['extra']['filters']['tag'])) {
        $element['extra']['filters']['tag'] = $this->importTag($element['extra']['filters']['tag']);
      }
      
      $return[$key] = $element;
    }
    return $return;
  }
  
  public function importCheckCiviElements(&$array) {
    $return = array();
    foreach($array as $key => $element) {
      $newKey = $key;
      $newElement = $element;
      if (strpos($key, 'group') === 0) {
        $newElement = $this->importGroup($newElement);
      } elseif (strpos($key, 'tag') === 0) {
        $newElement = $this->importTag($newElement);
      } elseif (is_array($element)) {
        $newElement = $this->importCheckCiviElements($array[$key]);
      }
      
      $return[$newKey] = $newElement;
    }
    return $return;
  }
  
  public function checkCiviElements(&$array) {
    $return = array();
    foreach($array as $key => $element) {
      $newKey = $key;
      $newElement = $element;
      if (strpos($key, 'group') === 0) {
        $newElement = $this->exportGroup($newElement);
      } elseif (strpos($key, 'tag') === 0) {
        $newElement = $this->exportTag($newElement);
      } elseif (is_array($element)) {
        $newElement = $this->checkCiviElements($array[$key]);
      }
      
      $return[$newKey] = $newElement;
    }
    return $return;
  }
  
  protected function exportGroup($groups) {
    $return = array();
    foreach($groups as $group_id) {
      $name = $this->getGroupName($group_id);
      if (strlen($name)) {
        $return[$name] = $name;
      }
    }
    return $return;
  }
  
  protected function importGroup($groups) {
    $return = array();
    foreach($groups as $group_name) {
      $id = $this->getGroupId($group_name);
      $return[$id] = $id;
    }
    return $return;
  }
  
  protected function getGroupName($group_id) {
    try {
      return civicrm_api3('Group', 'getvalue', array('id' => $group_id, 'return' => 'name'));
    } catch (Exception $ex) {
      //do nothing
    }
    return $group_id;
  }
  
  protected function getGroupId($name) {
    try {
      return civicrm_api3('Group', 'getvalue', array('name' => $name, 'return' => 'id'));
    } catch (Exception $ex) {
      //do nothing
    }
    return $name;
  }
  
  protected function exportTag($tags) {
    $return = array();
    foreach($tags as $tag_id) {
      $name = $this->getTagName($tag_id);
      if (strlen($name)) {
        $return[$name] = $name;
      }
    }
    return $return;
  }
  
  protected function importTag($tags) {
    $return = array();
    foreach($tags as $tag_name) {
      $id = $this->getTagId($tag_name);
      $return[$id] = $id;
    }
    return $return;
  }
  
  protected function getTagName($tag_id) {
    try {
      return civicrm_api3('Tag', 'getvalue', array('id' => $tag_id, 'return' => 'name'));
    } catch (Exception $ex) {
      //do nothing
    }
    return $tag_id;
  }
  
  protected function getTagId($name) {
    try {
      return civicrm_api3('Tag', 'getvalue', array('name' => $name, 'return' => 'id'));
    } catch (Exception $ex) {
      //do nothing
    }
    return $name;
  }
  

  
}