﻿// <auto-generated />
using System;
using Microsoft.EntityFrameworkCore;
using Microsoft.EntityFrameworkCore.Infrastructure;
using Microsoft.EntityFrameworkCore.Storage.ValueConversion;

#nullable disable

namespace Server.Migrations
{
    [DbContext(typeof(TestpersonContext))]
    partial class TestpersonContextModelSnapshot : ModelSnapshot
    {
        protected override void BuildModel(ModelBuilder modelBuilder)
        {
#pragma warning disable 612, 618
            modelBuilder.HasAnnotation("ProductVersion", "6.0.9");

            modelBuilder.Entity("UIAutomate.Models.SUSTest", b =>
                {
                    b.Property<int>("Id")
                        .ValueGeneratedOnAdd()
                        .HasColumnType("INTEGER");

                    b.Property<bool>("Agree")
                        .HasColumnType("INTEGER");

                    b.Property<bool>("Disagree")
                        .HasColumnType("INTEGER");

                    b.Property<bool>("Neutral")
                        .HasColumnType("INTEGER");

                    b.Property<string>("Question")
                        .HasColumnType("TEXT");

                    b.Property<bool>("StronglyAgree")
                        .HasColumnType("INTEGER");

                    b.Property<bool>("StronglyDisagree")
                        .HasColumnType("INTEGER");

                    b.Property<int?>("TestpersonId")
                        .HasColumnType("INTEGER");

                    b.HasKey("Id");

                    b.HasIndex("TestpersonId");

                    b.ToTable("SUSTests");
                });

            modelBuilder.Entity("UIAutomate.Models.Testperson", b =>
                {
                    b.Property<int>("Id")
                        .ValueGeneratedOnAdd()
                        .HasColumnType("INTEGER");

                    b.Property<string>("anonymName")
                        .HasColumnType("TEXT");

                    b.HasKey("Id");

                    b.ToTable("Testpersons");
                });

            modelBuilder.Entity("UIAutomate.Models.UEQTest", b =>
                {
                    b.Property<int>("Id")
                        .ValueGeneratedOnAdd()
                        .HasColumnType("INTEGER");

                    b.Property<string>("LeftWord")
                        .HasColumnType("TEXT");

                    b.Property<string>("RightWord")
                        .HasColumnType("TEXT");

                    b.Property<int?>("TestpersonId")
                        .HasColumnType("INTEGER");

                    b.Property<bool>("five")
                        .HasColumnType("INTEGER");

                    b.Property<bool>("four")
                        .HasColumnType("INTEGER");

                    b.Property<bool>("one")
                        .HasColumnType("INTEGER");

                    b.Property<bool>("seven")
                        .HasColumnType("INTEGER");

                    b.Property<bool>("six")
                        .HasColumnType("INTEGER");

                    b.Property<bool>("three")
                        .HasColumnType("INTEGER");

                    b.Property<bool>("two")
                        .HasColumnType("INTEGER");

                    b.HasKey("Id");

                    b.HasIndex("TestpersonId");

                    b.ToTable("UEQTests");
                });

            modelBuilder.Entity("UIAutomate.Models.VisAWITest", b =>
                {
                    b.Property<int>("Id")
                        .ValueGeneratedOnAdd()
                        .HasColumnType("INTEGER");

                    b.Property<bool>("Agree")
                        .HasColumnType("INTEGER");

                    b.Property<bool>("AgreeEtAll")
                        .HasColumnType("INTEGER");

                    b.Property<bool>("AgreeRather")
                        .HasColumnType("INTEGER");

                    b.Property<bool>("Disagree")
                        .HasColumnType("INTEGER");

                    b.Property<bool>("DisagreeEtAll")
                        .HasColumnType("INTEGER");

                    b.Property<bool>("DisagreeRather")
                        .HasColumnType("INTEGER");

                    b.Property<bool>("Neutral")
                        .HasColumnType("INTEGER");

                    b.Property<string>("Question")
                        .HasColumnType("TEXT");

                    b.Property<int?>("TestpersonId")
                        .HasColumnType("INTEGER");

                    b.HasKey("Id");

                    b.HasIndex("TestpersonId");

                    b.ToTable("VisAWITests");
                });

            modelBuilder.Entity("UIAutomate.Models.SUSTest", b =>
                {
                    b.HasOne("UIAutomate.Models.Testperson", "Testperson")
                        .WithMany("SUSTests")
                        .HasForeignKey("TestpersonId");

                    b.Navigation("Testperson");
                });

            modelBuilder.Entity("UIAutomate.Models.UEQTest", b =>
                {
                    b.HasOne("UIAutomate.Models.Testperson", "Testperson")
                        .WithMany("UEQTests")
                        .HasForeignKey("TestpersonId");

                    b.Navigation("Testperson");
                });

            modelBuilder.Entity("UIAutomate.Models.VisAWITest", b =>
                {
                    b.HasOne("UIAutomate.Models.Testperson", "Testperson")
                        .WithMany("VisAWITests")
                        .HasForeignKey("TestpersonId");

                    b.Navigation("Testperson");
                });

            modelBuilder.Entity("UIAutomate.Models.Testperson", b =>
                {
                    b.Navigation("SUSTests");

                    b.Navigation("UEQTests");

                    b.Navigation("VisAWITests");
                });
#pragma warning restore 612, 618
        }
    }
}
