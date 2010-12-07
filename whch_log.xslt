<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:output method="text"/>

<xsl:template match="/" indent="yes" >
    <xsl:text>
    {| border="1" cellpadding="4" class="wikitable"
    </xsl:text>
    <xsl:text>! Client!! Duration!! Task!! Details</xsl:text>
    <xsl:for-each select="year/day/task">
    <xsl:sort select="@client"/>
    <xsl:text>
    |-
    </xsl:text>
      <xsl:text>|</xsl:text>
      <xsl:value-of select="@client"/>
      <xsl:text>||</xsl:text>
      <xsl:value-of select="@client"/>
      <xsl:text>||</xsl:text>

      <!-- Start: Calculate time difference -->
      <!-- Need to find a good algorithem still. -->
      <xsl:variable name="start" select="@start"/>
      <xsl:variable name="startTime" select="substring-after($start,'T')"/>
      <xsl:variable name="startHours" select="substring-before($startTime,':')"/>
      <xsl:variable name="startMinutes" select="substring($startTime,4,2)"/>
      <xsl:variable name="startSeconds" select="substring($startTime,7,2)"/>

      <xsl:variable name="end" select="@end"/>
      <xsl:variable name="endTime" select="substring-after($end,'T')"/>
      <xsl:variable name="endHours" select="substring-before($endTime,':')"/>
      <xsl:variable name="endMinutes" select="substring($endTime,4,2)"/>
      <xsl:variable name="endSeconds" select="substring($endTime,7,2)"/>

      <xsl:variable name="sHours" select="number($startHours)"/>
      <xsl:variable name="eHours" select="number($endHours)"/>
      <xsl:variable name="Hours" select="$eHours - $sHours"/>

      <xsl:variable name="sMinutes" select="number($startMinutes)"/>
      <xsl:variable name="eMinutes" select="number($endMinutes)"/>
      <xsl:variable name="Minutes" select="$eMinutes - $sMinutes"/>

      <xsl:variable name="sSeconds" select="number($startSeconds)"/>
      <xsl:variable name="eSeconds" select="number($endSeconds)"/>
      <xsl:variable name="Seconds" select="$eSeconds - $sSeconds"/>

      <xsl:variable name="duration" select="concat($Hours,':',$Minutes, ':', $Seconds)"/>
      <xsl:value-of select="$duration"/>
      <!-- End: Calculate time difference -->

      <xsl:text>||</xsl:text>
      <xsl:value-of select="details"/>
    </xsl:for-each>
    <xsl:text>
    |}
    </xsl:text>
</xsl:template>
</xsl:stylesheet>

